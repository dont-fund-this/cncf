<?php
class Triplet {
    public string $address;
    public string $payload;
    public string $options;

    public function __construct(string $address, string $payload, string $options) {
        $this->address = $address;
        $this->payload = $payload;
        $this->options = $options;
    }
}
