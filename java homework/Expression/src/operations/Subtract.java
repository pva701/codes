package operations;

import exceptions.OverflowArithmeticException;
import exceptions.UnexpectedTokenException;
import operations.BinaryOperation;
import operations.Expression3;
import types.INumber;

/**
 * Created by pva701 on 3/15/14.
 */
public class Subtract<T extends INumber<T> > extends BinaryOperation<T> {

    public Subtract(Expression3<T> xx, Expression3<T> yy) {
        super(xx, yy);
    }

    @Override
    T op(T valX, T valY) throws OverflowArithmeticException {
        return valX.sub(valY);
    }

    @Override
    public String signOperation() {
        return "-";
    }
}
