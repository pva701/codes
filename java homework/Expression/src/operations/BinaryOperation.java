package operations;


import types.INumber;

/**
 * Created by pva701 on 3/15/14.
 */

public abstract class BinaryOperation<T extends INumber<T> > implements Expression3<T> {
    protected Expression3<T> first, second;
    BinaryOperation(Expression3<T> xx, Expression3<T> yy) {
        first = xx;
        second = yy;
    }

    @Override
    public T evaluate(T x, T y, T z) {
        T valX = first.evaluate(x, y, z);
        T valY = second.evaluate(x, y, z);
        return op(valX, valY);
    }
    abstract T op(T valX, T valY);
    public abstract String signOperation();
}
