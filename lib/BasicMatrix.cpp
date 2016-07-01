#include "BasicMatrix.h"

BasicMatrix::BasicMatrix(unsigned int rows, unsigned int columns)
{
    this->rows = rows;
    this->columns = columns;
    
    this->m = (float*) calloc( this->columns*this->rows, sizeof(float) );
    
    // If this BasicMatrix is square, initialize it to an identity matrix.
    if(this->columns == this->rows)
    {
        for( unsigned int i = 0; i < this->columns; ++i )
        {
            this->m[ this->columns*i + i ] = 1.0;
        }
    }
}

BasicMatrix::~BasicMatrix()
{
    free (m);
}

float BasicMatrix::dotProduct(float * a, float * b, unsigned int len)
{
    float result = 0;
    for( unsigned int i = 0; i < len; ++i )
    {
        result += a[i]*b[i];
    }
    return result;
}

unsigned int BasicMatrix::getRows()
{
    return this->rows;
}

unsigned int BasicMatrix::getColumns()
{
    return this->columns;
}

void BasicMatrix::getRow(unsigned int r, float * row)
{
    for( int i = 0; i < this->columns; ++i )
    {
        (row)[i] = m[ r*this->columns + i ];
    }
}

void BasicMatrix::getColumn(unsigned int c, float * column)
{
    for( unsigned int i = 0; i < this->rows; ++i )
    {
        column[i] = m[ i*this->columns + c ];
    }
}

float BasicMatrix::get(unsigned int r, unsigned int c)
{
    return this->m[ r*this->columns + c ];
}

void BasicMatrix::set(unsigned int r, unsigned int c, float val)
{
    this->m[ r*this->columns + c ] = val;
}

void BasicMatrix::set(BasicMatrix * other)
{
    // If the dimensions aren't the same, we can't really do anything.
    if(this->columns != other->columns || this->rows != other->rows) return;
    
    // Otherwise we copy the underlying array.
    for( unsigned int i = 0; i < this->columns*this->rows; ++i )
    {
        this->m[i] = other->m[i];
    }
}

void BasicMatrix::multiplyBy(BasicMatrix* rhs)
{
    // Do dimension test. If it fails, we don't do anything to
    // this matrix.
    if( this->columns != rhs->rows ) return;
    
    // Create temporary arrays to store rows and columns.
    float * row = (float*) malloc( sizeof(float)*this->columns );
    float * column = (float*) malloc( sizeof(float)*rhs->rows );
    
    // Go through each row, and compute the dot product between it
    // and the corresponding column of the right-hand-side matrix.
    for( unsigned int r = 0; r < this->rows; ++r )
    {
        // Get the current row.
        this->getRow(r, row);
        // For each item in the row...
        for( unsigned int c = 0; c < this->columns; ++c )
        {
            // Get the corresponding column...
            rhs->getColumn(c, column);
            // And compute the dot product.
            this->m[ r*this->columns + c ] = BasicMatrix::dotProduct(row, column, this->columns);
        }
    }
    
    // Free the memory held by the temporary arrays.
    free(row);
    free(column);
}

float * BasicMatrix::getLinear()
{
    return this->m;
}
