nrnmpiSRC='nrnmpi mpispike'

nrnocSRC='capac eion finitialize fadvance_core solve_core treeset_core
  nrnoc_aux register_mech multicore nrntimeout'

nrnocModSRC='stim hh'

nrnivSRC='netpar netcvode cxprop cvodestb tqueue htlist ivlistimpl ivvect
  mk_mech nrn_setup output_spikes'

rm -r -f *.o

for i in $nrnmpiSRC ; do
  mpicc -g -I. -Inrnoc -Inrniv -c nrnmpi/$i.c
done

for i in $nrnocSRC ; do
  clang -g -I. -Inrnoc -Inrniv -c nrnoc/$i.c
done

for i in $nrnocModSRC ; do
  gcc -g -I. -Inrnoc -Inrniv -c nrnoc/$i.c
done

for i in $nrnivSRC ; do
  clang++ -g -I. -Inrnoc -Inrniv -c nrniv/$i.cpp
done

g++ -I. -Inrnoc -Inrniv -c main.cpp

mpicxx *.o -lm -pthread
