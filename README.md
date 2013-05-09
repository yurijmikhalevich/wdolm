wdolm — Wake/Down-On-Lan Manager, cross-platform application to power-on/shutdown (only windows) computers in local network.

Basicaly written for KubSAU.

Shutdown works through netrpc, so on linux and MacOS X it requires installed samba to work.

Computers should be configured in computers.txt file, located in the same directory, as the executable.

Example of computers.txt file:

c MAC comp_name

c MAC comp_name

1 Floor

c MAC comp_on_floor

11 Room

c MAC comp_in_room

There should be no empty lines in file.
