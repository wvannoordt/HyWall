#ifndef INDEXING_H
#define INDEXING_H

#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define elem(...) GET_MACRO(__VA_ARGS__, elem2, elem1)(__VA_ARGS__)
#define elem1(myarr, myglobalidx) myarr[(myglobalidx)]
#define elem2(myarr, myglobalidx, mylocalidx) myarr[(myglobalidx)*N + (mylocalidx)]

#define localtriple(mytrip, myarr, myglobalidx, mylocalidx) dvec3 mytrip; mytrip[0] = elem(myarr, myglobalidx, mylocalidx-1); mytrip[1] = elem(myarr, myglobalidx, mylocalidx); mytrip[2] = elem(myarr, myglobalidx, mylocalidx+1);

#endif
