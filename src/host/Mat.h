#ifndef HY_MAT_H
#define HY_MAT_H
#include <vector>
#include <stdexcept>
namespace HyWall
{
    template <class myClass> class Mat
    {
        public:
            Mat(size_t rows_in, size_t cols_in)
            {
                rows.resize(rows_in);
                for (auto& c:rows) c.resize(cols_in);
                colsCount = cols_in;
                rowsCount = rows_in;
            }
            
            Mat(void)
            {
                colsCount = 0;
                rowsCount = 0;
            }
            
            void fill(myClass val)
            {
                for (auto& c:rows)
                {
                    for (auto& e:c)
                    {
                        e = val;
                    }
                }
            }
            
            size_t size(int dir)
            {
                switch (dir)
                {
                    case 0: return rowsCount;
                    case 1: return colsCount;
                    default:
                    {
                        throw std::out_of_range("Mat::size : index is out of range");
                    }
                }
                return 0;
            }
            
            std::vector<myClass>& operator [] (size_t i) {return rows[i];}
            myClass& operator () (size_t i, size_t j) {return rows[i][j];}
            
        private:
            std::vector<std::vector<myClass>> rows;
            size_t rowsCount, colsCount;
        
    };
}

#endif