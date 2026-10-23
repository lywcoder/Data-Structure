#include "sparseMatrix.h"
#include <algorithm>

SparseMatrix::SparseMatrix(int r, int c) : rows(r), cols(c) {}

//操作1：重置矩阵
void SparseMatrix::reset(int r, int c) {
    rows = r;
    cols = c;
    terms.clear();

    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            int val;
            std::cin >> val;
            if (val != 0) {
                terms.push_back({i, j, val});
            }
        }
    }
}

//读入三元组输入的矩阵
void SparseMatrix::readSparse(int r, int c, int t) {
    rows = r;
    cols = c;
    terms.clear();
    terms.reserve(t);

    for (int i; i < t; ++i) {
        int x, y, v;
        std::cin >> x >> y >> v;
        if (v != 0) {
            terms.push_back({x, y, v});
        }
    }
}

//操作2：稀疏矩阵的乘法: 先组织好一行和一列，然后再去匹配相同的下标(原来的Q是行主序存储，但是需要取乘法Q的列)
//关键就是去一行一列做点积，并且0元没有贡献,只需要找到元素位置下标对应相同即可 //统计Q的每列元素的数量
bool SparseMatrix::multiply(const SparseMatrix& Q) { 
    // 判断乘法是否合法
    if (cols != Q.rows) {
        *this = Q;
        return false;
    } //矩阵性质
    
    //1. 统计Q矩阵的每列非零元素个数(原来的Q是行主序存储，但是需要取乘法Q的列)
    std::vector<int> qColCount(Q.cols + 1, 0); //统计Q的每列元素的数量
    for (const auto& term : Q.terms) { 
        qColCount[term.col]++;
    }

    //2. 计算Q矩阵每列的起始位置索引,使用前缀后计算每列起始位置的索引(类似矩阵的转置)
    std::vector<int> qColStart(Q.cols + 2, 0);
    for (int c = 1; c <= Q.cols; ++c) { //计算每列的起始位置
        qColStart[c + 1] = qColStart[c] + qColCount[c];
    }
    //以上都是复用的转置矩阵的方法，但是没有交换行列坐标

    //3. 构建Q的按列排序的三元组数组qTransposedTerms
    std::vector<MatrixTerm> qColTerms(Q.terms.size());// 先按照列排序，同一列内部按照行排序-》行主序变为列主序
    std::vector<int> currentPos = qColStart; 
    
    //将元素分配到对应的区段,类似转置之后做点积, currentPos已经保存好对应的每列的起始位置索引了
    for (const auto& term: Q.terms) {
        int dest = currentPos[term.col]; //currentPos保存的是下一个待分配的下标
        currentPos[term.col]++; //填充完之后就后移动, 复用快速矩阵的转置的方法，但是没有交换行列坐标
        qColTerms[dest] = term; //将元素填入对应的位置,重新排列，但是保留原坐标, 按照列排序，但是同一列内存按照行排列
    }

    //4. 统计P矩阵每行的起始索引，方便按照行来扫描P
    std::vector<int> pRowCount(rows + 1, 0);
    for (const auto& term : terms) {
        pRowCount[term.row]++; //每一行的元素个数,为每一行的P来简历边界，方便取行做点积
    }

    //给p的每一行建立边界，本来p就是使用的行主排列，不需要重排, 只需要知道每一行从哪里开始从哪里结束
    std::vector<int> pRowStart(rows + 2, 0);
    for (int r = 1; r <= rows; ++r) {
        pRowStart[r + 1] = pRowStart[r] + pRowCount[r]; //同样使用前缀和来计算下一行初始的位置,这样可以直接找到每一行，不用再重新扫描
    }
    //p的一行是一个连续空间，Q的一列是一个连续空间

    //保存结果数组
    std::vector<MatrixTerm> result;

    //5. 按照行-列来计算P * Q
    for (int r = 1; r <= rows; ++r) {
        int pStart = pRowStart[r]; //确定区间，左开右闭
        int pEnd = pRowStart[r + 1];
        if (pStart == pEnd) continue; // P的第r行为空, 可以跳过空行空列

        for (int c = 1; c <= Q.cols; ++c) {
            int qStart = qColStart[c]; //确定区间，左开右闭
            int qEnd = qColStart[c + 1];
            if (qStart == qEnd) continue; // Q的第c列为空

            //双指针计算P的第r行和Q的第c列的点积
            int sum = 0;
            int pIdx = pStart; //同样使用归并的双指针
            int qIdx = qStart; //使用的是中间下标

            while (pIdx < pEnd && qIdx < qEnd) {
                //中间下标是否相等
                if (terms[pIdx].col < qColTerms[qIdx].row) {
                    pIdx++;
                } else if (terms[pIdx].col > qColTerms[qIdx].row) {
                    qIdx++;
                } else { //之后terms.col = qcol.row相等(中间的遍历下标相等）才能对最后的点积有贡献，否则无效
                    sum += terms[pIdx].value * qColTerms[qIdx].value;//int自然溢出
                    pIdx++;
                    qIdx++;
                }
            }

            if (sum != 0) { //如果点积不为0，存入结果矩阵
                result.push_back({r, c, sum});
            }
        }
    }

    rows = this->rows; //行数是原来P的行数，列数是Q的列数
    cols = Q.cols; //得出结果的矩阵大小
    
    terms = std::move(result); //进行移动赋值得到结果矩阵
    return true;
}

// 操作3：稀疏矩阵的加法(归并双指针), 类似链表的归并排序
// 将矩阵相加变为两个有序数组的归并
bool SparseMatrix::add(const SparseMatrix& Q) {
    //判断加法是否合法
    if (rows != Q.rows || cols != Q.cols) {
        *this = Q;
        return false;
    }//矩阵大小需要合法

    std::vector<MatrixTerm> result;
    size_t i = 0, j = 0; // i是P中第一个还没有处理的元素，j是Q中第一个还没有处理的元素

    //使用主序归并两个三元组数组
    while (i < terms.size() && j < Q.terms.size()) {
        int posP = (terms[i].row - 1) * cols + terms[i].col; //Pterms的对应现实矩阵中的元素位置
        int posQ = (Q.terms[j].row - 1) * cols + Q.terms[j].col; //Qterms的对应现实矩阵中的元素位置
        //两个数组都是行主序，可以从头扫描两个数组

        if (posP < posQ) {
            result.push_back(terms[i++]);
        } else if (posP > posQ) {
            result.push_back(Q.terms[j++]);
        } else { //只有位置相等才能相加
            int sumVal = terms[i].value + Q.terms[j].value; //int自然溢出 
            if (sumVal != 0) { //如果相加为0则不存入元素
                result.push_back({terms[i].row, terms[i].col, sumVal});
            }
            i++; //扫描定位指针后移,进行下面需要处理元素
            j++;
        }
    }

    //处理完之后，直接后面追加就行，因为另一边都是0了
    while (i < terms.size()) result.push_back(terms[i++]);
    while (j < Q.terms.size()) result.push_back(Q.terms[j++]);
    
    //最后将结果进行移动
    terms = std::move(result);
    return true;
}

//操作5：稀疏矩阵快速转置
//可以先找原矩阵第一列的所有元素，以此类推,结果一定有序
//可以不用每次从头扫描对应的数组，只需要直接先算出每一列
//对应的元素放到结果数组的那一段，然后扫描一次将元素直接放进去 ->计数， 前缀和，分配位置

void SparseMatrix::transpose() {
    if (terms.empty()) {
        std::swap(rows, cols); //交换行列
        return;
    }

    std::vector<int> colCount(cols + 1, 0); //统计原矩阵每列有多少个非0元素, 其中0下标不用
    for (const auto& term : terms) {
        colCount[term.col]++; //term在容器vector terms中进行遍历,计算需要转置的矩阵每一列有多少个元素需要存
    }

    std::vector<int> rowStart(cols + 2, 0); //前缀和算出每一行的起点,最后一次循环数组会访问rowStart[col + 1]
    for (int c = 1; c <= cols; ++c) {
        //前缀和计算每行的起点
        //并且元素采用左闭右开的区间：rowStart[c]~rowStart[c + 1] 
        rowStart[c + 1] = rowStart[c] + colCount[c]; //下一行的起点 = 当前行的起点 + 当前行的非零的元素数量
    }

    std::vector<MatrixTerm> transposed(terms.size()); //分配转置矩阵空间的大小
    std::vector<int> currentPos = rowStart; //rowStart[r]是每行固定的起始位置, currentPos是下一个可以写入的位置
    //开始的时候一行没有放任何元素，下一个位置就是起点

    for (const auto& term : terms) {
        int dest = currentPos[term.col]; //先取本行的一个空位,再把指针空位往后移动
        currentPos[term.col]++;
        transposed[dest] = {term.col, term.row, term.value};
    }

    std::swap(rows, cols); //交换原来矩阵的行数和列数,
    terms = std::move(transposed); //移动赋值原来的三元组数组
}

// 操作4：按照阵列输出矩阵
void SparseMatrix::print() const {
    std::cout << rows << " " << cols << '\n';
    size_t idx = 0; //size_t是表示大小的类型
    for (int i = 1; i <= rows; ++i) {
        for (int j  = 1; j <= cols; ++j) {
            if (idx < terms.size() && terms[idx].row == i && terms[idx].col == j) {
                std::cout << terms[idx].value; //输出实际矩阵的每一个值
                idx++;
            } else {
                std::cout << 0;
            } 
            if (j < cols) std::cout << " ";
        }
        std::cout << '\n';
    }
}