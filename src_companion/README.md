## Original Project
The guides from the mongoose documentation are useful guides for navigating this project.
> https://mongoose.ws/documentation/tutorials/rp2040/pico-w/

> https://github.com/mongoose-examples/raspberry-pi-pico-w/blob/5bfae33/lwipopts.h#L86-L92


## Dependencies
Make sure you have `docker`, `gcc` and `make` installed on your system. 
## Compress Web Assets
Install `gzip` on your platform, then run gzip on your web artificts like `.html` and `.js`. Don't bother with things like images because they are usually already compressed.

## Pack Web Assets
1. Compile `pack.c` into a binary that runs on your local system with `cc -o pack pack.c`
2. Then run pack your assets in the folder `./web_root` i.e. `./pack <List of filenames>` 

## Building UF2 for Pi Pico
Use `make build` to compile the project.

## General Outline
1. `main.c` is where you define the task methods, their priorities and where you dispatch the tasks. It also contains some code to enable a DHCP server and hotspot for the pi pico.
2. `net.c` All of the web server API stuff goes in here.
