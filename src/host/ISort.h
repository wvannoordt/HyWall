#ifndef ISORT_H
#define ISORT_H
#include <vector>
#include <algorithm>
template <typename sortType> struct SortPair
{
    SortPair(int i, sortType val) {permIdx = i; sortValue = val;}
    int permIdx;
    sortType sortValue;
    bool operator<( const SortPair& rhs ) const
    {
        return sortValue < rhs.sortValue;
    }
};

template <typename sortType, class myClass> void ISort(std::vector<sortType>& idx, std::vector<myClass>& vals)
{
    std::vector<SortPair<sortType>> perm;
    int c = 0;
    for (const auto& i:idx) perm.push_back(SortPair<sortType>(c++, i));
    std::sort(perm.begin(), perm.end());
    std::vector<myClass> newVals;
    for (int c = 0; c < perm.size(); c++)
    {
        newVals.push_back(vals[perm[c].permIdx]);
    }
    vals = newVals;
    std::sort(idx.begin(), idx.end());
}

template <class myClass> void ISort(std::vector<myClass>& vals)
{
    std::sort(vals.begin(), vals.end());
}

#endif