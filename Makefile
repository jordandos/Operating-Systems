shm_proc: shm_processes.c
gcc shm_processes.c -D_SVID_SOURCE -D_GNU_SOURCE -Wall -std=c99 -Werror=cpp -pedantic -o shm_proc

bank_simulation: bank_simulation.c
gcc bank_simulation.c -o bank_simulation -lpthread

run_bank_simulation: bank_simulation
./bank_simulation
