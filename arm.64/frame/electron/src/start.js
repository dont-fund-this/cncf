import { protocol } from 'electron';
import { post } from './post.js';
import { show } from "./show.js";

const PRIVILEGED = { standard: true, secure: true, supportFetchAPI: true, corsEnabled: true };

protocol.registerSchemesAsPrivileged([
  { scheme: 'efs', privileges: PRIVILEGED },
]);

const MIME = {
  html: 'text/html', htm: 'text/html', css: 'text/css',
  js: 'application/javascript', mjs: 'application/javascript',
  json: 'application/json', svg: 'image/svg+xml',
  png: 'image/png', jpg: 'image/jpeg', jpeg: 'image/jpeg',
  gif: 'image/gif', webp: 'image/webp', wasm: 'application/wasm',
  ico: 'image/x-icon', woff: 'font/woff', woff2: 'font/woff2',
  ttf: 'font/ttf', txt: 'text/plain', md: 'text/plain',
};

export function start(capture) {

  protocol.handle('efs', async (request) => {
    const u = new URL(request.url);
    const path = (u.host + u.pathname).replace(/^\/+/, '').split(/[?#]/, 1)[0];
    if (!path) return new Response('', { status: 404 });

    let reply;
    try {
      const [pkt] = await post('efs.read', { path }, { once: true, timeout: 20 });
      if (!pkt) return new Response('', { status: 404 });
      reply = JSON.parse(pkt.payload);
    } catch { return new Response('', { status: 502 }); }

    if (!reply.ok) return new Response('', { status: 404 });
    const bytes = Buffer.from(reply.data ?? '', 'base64');
    const dot = path.lastIndexOf('.');
    const type = dot < 0
      ? 'application/octet-stream'
      : MIME[path.slice(dot + 1).toLowerCase()] ?? 'application/octet-stream';
    return new Response(bytes, { status: 200, headers: { 'Content-Type': type } });
  });

  show(capture);
}
