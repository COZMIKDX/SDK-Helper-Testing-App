# SDK Helper Test App

This is my MGS_PEBBLE watchface but I am using it as a testing ground for my Pebble SDK Helper Library.

Image allocation must be done before Text allocation. Probably due to memory being too segmented when doing several of the smaller Text allocations first.
