import { app } from 'electron';

import { open  } from './open.js';
import { stop  } from './stop.js';
import { start } from './start.js';

app.disableHardwareAcceleration();

const argv    = process.argv;
const shotAt  = argv.indexOf('--shot');
const libAt   = argv.indexOf('--libdir');
const capture = shotAt >= 0 ? argv[shotAt + 1] : undefined;
const libDir  = libAt  >= 0 ? argv[libAt  + 1] : undefined;

app.whenReady().then(() => {
 open(libDir);
 start(capture);
});

app.on('will-quit', () => stop());

app.on('window-all-closed', () => app.quit());
