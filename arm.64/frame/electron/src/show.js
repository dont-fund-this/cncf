import { BrowserWindow, screen, app } from 'electron';
import { writeFileSync } from 'node:fs';

const CONFIG = {
    url   : 'efs://html/main/index.html',
    title : 'pat',
    width : 960,
    height: 600
};

export function show(capture) {
  if (capture) {
    const off = new BrowserWindow({
      width: CONFIG.width,
      height: CONFIG.height,
      show: false,
      webPreferences: { nodeIntegration: false, contextIsolation: true, offscreen: true },
    });
    let latest = null;
    off.webContents.on('paint', (_event, _dirty, image) => { latest = image; });
    off.webContents.once('did-finish-load', () => setTimeout(() => {
      if (latest) writeFileSync(capture, latest.toPNG());
      app.quit();
    }, 1200));
    off.loadURL(CONFIG.url);
    return;
  }

  const { workArea } = screen.getPrimaryDisplay();
  const win = new BrowserWindow({
    width: CONFIG.width,
    height: CONFIG.height,
    x: Math.round(workArea.x + (workArea.width - CONFIG.width) / 2),
    y: Math.round(workArea.y + (workArea.height - CONFIG.height) / 2),
    title: CONFIG.title,
    backgroundColor: '#101418',
    show: false,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      devTools: true,
    },
  });
  win.loadURL(CONFIG.url);
  win.once('ready-to-show', () => win.show());
}
