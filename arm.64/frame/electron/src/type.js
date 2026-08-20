import koffi from 'koffi';

koffi.struct('Info', {
  plugin_type: 'const char*',
  product: 'const char*',
  description_long: 'const char*',
  description_short: 'const char*',
  plugin_id: 'unsigned long',
});

koffi.proto('int CallbackFn(const char* address, const char* payload, const char* options)');
