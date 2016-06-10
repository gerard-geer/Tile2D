#ifndef BASICMATRIX_H
#define BASICMATRIX_H

#include <cstdlib>

// Todo: make faster by means that this-> may be slower than not using it.

/**
 * @class BasicMatrix
 * @author Gerard Geer
 * @date 06/01/16
 * @file BasicMatrix.h
 * @brief A very simple class that simplifies working with matrices within the
 *        scope of cooking up matrices for OpenGL programming.
 */
class BasicMatrix
{
// Friends with itself so we can access private members of other instances
// in member functions.
friend class BasicMatrix;
private:

    /*
     * The underlying one dimensional array.
     */
    float* m;
    
    /*
     * The number of rows in the matrix.
     */
    unsigned int rows;
    
    /*
     * The number of columns in the matrix.
     */
    unsigned int columns;
    
    /**
     * @brief Computes the dot product between two vectors. This is used
     *        for multiplying the column with row during matrix multiplication.
     * @param a The first vector.
     * @param b The second vector.
     * @param len The length of either vector, common to each.
     * @return The dot product between the two given vectors.
     */
    static float dotProduct(float * a, float * b, unsigned int len);

public:
    /**
     * @brief Constructs the BasicMatrix. This is the only time the dimensions
     *        of the matrix can be set. The values of the matrix are initialized
     *        to an identity matrix if square.
     */
    BasicMatrix(unsigned int rows, unsigned int columns);
    
    /**
     * @brief Destructs the BasicMatrix. Frees the underlying memory.
     */
    ~BasicMatrix();
    
    /**
     * @brief Returns the number of rows (height) of this BasicMatrix.
     * @return The number of rows of this BasicMatrix.
     */
    unsigned int getRows();
    
    /**
     * @brief Returns the number of columns (width) of this BasicMatrix.
     * @return The number of columns in this BasicMatrix.
     */
    unsigned int getColumns();
    
    /**
     * @brief Populates the given array with the values of the requested row.
     *        Note, there must be enough space to store the values.
     * @param r The row requested.
     * @param row The pre-created array to store the values in.
     */
    void getRow(unsigned int r, float * row);
    
    /**
     * @brief Populates the given array with the values of the requested column.
     *        Note, there must be enough space to store the values.
     * @param c The column requested.
     * @param column The pre-created array to store the values in.
     */
    void getColumn(unsigned int c, float * column);
    
    /**
     * @brief Returns a particular value of this BasicMatrix.
     * @param r The row of the item desired.
     * @param c The column of the item desired.
     * @return A particular value of the BasicMatrix.
     */
    float get(unsigned int r, unsigned int c);
    
    /**
     * @brief Sets a particular value of this BasicMatrix.
     * @param r The row of the item to set.
     * @param c The column of the item to set.
     */
    void set(unsigned int r, unsigned int c, float val);
    
    /**
     * @brief Multiplies this BasicMatrix by another one.
     * @param rhs The BasicMatrix to be on the right hand side of the operation.
     */
    void multiplyBy(BasicMatrix * rhs);
    
    /**
     * @brief Returns a pointer to the underlying 1D array. Is it safe?
     *        Not one bit! Is it fast! a lot faster than creating a new
     *        array from a 2D one!
     * @return A pointer to the underlying 1D array.
     */
    float * getLinear();

};

#endif // BASICMATRIX_H
