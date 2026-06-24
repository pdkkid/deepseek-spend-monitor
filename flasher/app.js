import { ESPLoader, Transport } from 'esptool';

const FIRMWARE_URL = 'firmware.bin';
const FLASH_ADDR  = 0x0000;
const BAUD_RATE   = 921600;

let device = null;
let transport = null;
let loader = null;

const $ = (s) => document.querySelector(s);
const btnConnect = $('#btn-connect');
const btnFlash   = $('#btn-flash');
const stepFlash  = $('#step-flash');
const stepDone   = $('#step-done');
const chipInfo   = $('#chip-info');
const fwSize     = $('#fw-size');
const progressBar = $('#progress-bar');
const progressFill = $('#progress-fill');
const connectStatus = $('#connect-status');
const flashStatus   = $('#flash-status');

// Fetch firmware metadata
(async () => {
  try {
    const resp = await fetch(FIRMWARE_URL, { method: 'HEAD' });
    const len = parseInt(resp.headers.get('Content-Length') || '0');
    fwSize.textContent = (len / 1024 / 1024).toFixed(2) + ' MB';
  } catch { fwSize.textContent = 'unavailable'; }
})();

// Connect
btnConnect.addEventListener('click', async () => {
  btnConnect.disabled = true;
  connectStatus.textContent = 'Requesting serial port...';
  connectStatus.className = 'status';

  try {
    device = await navigator.serial.requestPort();
    await device.open({ baudRate: 115200 });

    transport = new Transport(device);
    loader = new ESPLoader({
      transport,
      baudrate: BAUD_RATE,
      terminal: { clean: () => {}, writeLine: (d) => console.log(d) },
    });

    connectStatus.textContent = 'Detecting chip...';
    const chip = await loader.main();
    chipInfo.textContent = `${chip}`;
    connectStatus.textContent = 'Connected — ' + chip;
    connectStatus.className = 'status ok';

    stepFlash.classList.remove('hidden');
    btnFlash.disabled = false;
  } catch (err) {
    connectStatus.textContent = 'Error: ' + (err.message || err);
    connectStatus.className = 'status err';
    btnConnect.disabled = false;
  }
});

// Flash
btnFlash.addEventListener('click', async () => {
  btnFlash.disabled = true;
  flashStatus.textContent = 'Downloading firmware...';
  flashStatus.className = 'status';
  progressBar.classList.remove('hidden');
  progressFill.style.width = '0%';

  try {
    const resp = await fetch(FIRMWARE_URL);
    if (!resp.ok) throw new Error('HTTP ' + resp.status);

    const total = parseInt(resp.headers.get('Content-Length') || '0');
    const reader = resp.body.getReader();
    const chunks = [];
    let received = 0;

    while (true) {
      const { done, value } = await reader.read();
      if (done) break;
      chunks.push(value);
      received += value.length;
      if (total) {
        progressFill.style.width = Math.round((received / total) * 50) + '%';
      }
    }

    const firmware = new Uint8Array(received);
    let pos = 0;
    for (const c of chunks) { firmware.set(c, pos); pos += c.length; }

    flashStatus.textContent = 'Flashing... (this takes ~30s)';
    progressFill.style.width = '50%';

    await loader.writeFlash({
      fileArray: [{ address: FLASH_ADDR, data: firmware.buffer }],
      flashSize: 'keep',
      flashMode: 'dio',
      flashFreq: 'keep',
      eraseAll: false,
      compress: true,
      reportProgress: (fileIdx, written, total) => {
        progressFill.style.width = 50 + Math.round((written / total) * 50) + '%';
      },
    });

    progressFill.style.width = '100%';
    flashStatus.textContent = 'Flash complete!';
    flashStatus.className = 'status ok';
    stepDone.classList.remove('hidden');

    await transport.waitForUnlock(5000);
    await device.close();
  } catch (err) {
    flashStatus.textContent = 'Error: ' + (err.message || err);
    flashStatus.className = 'status err';
    btnFlash.disabled = false;
    progressBar.classList.add('hidden');
  }
});
