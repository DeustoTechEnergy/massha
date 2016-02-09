#include "LeastSquaresSolvingSkill.h"

LeastSquaresSolvingSkill::LeastSquaresSolvingSkill() : Skill(){
}


LeastSquaresSolvingSkill::~LeastSquaresSolvingSkill(){
}

/**********************************************************************
 METHODS
**********************************************************************/

/**
 * @brief LeastSquaresSolving3
 * @param matrix First dimension rows, second dimension columns
 * @param values First dimension rows
 *      matrix       values
 *  [ a , b , c ]  =  [ w ]
 *  [ d , e , f ]  =  [ x ]
 *  [ g , h , i ]  =  [ y ]
 *  [ j , k , l ]  =  [ z ]
 * @return
 */
QVector<double> LeastSquaresSolvingSkill::leastSquaresSolving(QVector<QVector<double> > matrix, QVector<double> values){
    QVector<double> solution;

    if( matrix.isEmpty() || values.isEmpty() ){
        qWarning() << "[LeastSquaresSolving::LeastSquaresSolving] Empty matrix or values";
        return solution;
    }

    if( matrix.size() != values.size() ){
        qWarning() << "[LeastSquaresSolving::LeastSquaresSolving] Different size matrix and values";
        return solution;
    }

    // Matrix (Rows , Columns)
    Eigen::MatrixXd eigen_matrix( matrix.size(), matrix.first().size() );

    for(int r = 0; r < matrix.size(); r++){
        for(int c = 0; c < matrix.first().size(); c++){
            eigen_matrix(r, c) = matrix.at(r).at(c);
        }
    }

    JacobiSVD<MatrixXd> svd(eigen_matrix, ComputeThinU | ComputeThinV);

    // Values (Rows)
    Eigen::VectorXd eigen_vector( values.size() );

    for(int r = 0; r < values.size(); r++){
        eigen_vector(r) = values.at(r);
    }

    // Solve Least Squares
    Eigen::VectorXd solved = svd.solve( eigen_vector );

    for(int i = 0 ; i < solved.rows() ; i++){
        solution.append( solved.coeff(i) );
    }

    return solution;
}
