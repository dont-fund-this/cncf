<?php
function bind(string $binaryPath): ?object {
    $filename = basename($binaryPath);
    $skips = ['c', 'cpp', 'rust', 'go', 'swift', 'haskell', 'zig', 'v', 'slint_sample'];
    if (in_array($filename, $skips)) return null;

    try {
        $ffi = FFI::cdef("
            typedef const char* Address;
            typedef const char* Payload;
            typedef const char* Options;
            int Pump(Address address, Payload payload, Options options);
        ", $binaryPath);

        return (object)[
            'name' => $filename,
            'path' => $binaryPath,
            'ffi'  => $ffi
        ];
    } catch (Throwable $e) {
        return null;
    }
}
