package operations;

import exceptions.DivisionByZeroArithmeticException;
import exceptions.OverflowArithmeticException;
import operations.BinaryOperation;
import types.INumber;

/**
 * Created by pva701 on 3/15/14.
 */
public class Divide<T extends INumber<T> > extends BinaryOperation<T> {
    public Divide(Expression3<T> xx, Expression3<T> yy) {
        super(xx, yy);
    }

    @Override
    T op(T valX, T valY)
            throws DivisionByZeroArithmeticException, OverflowArithmeticException {
        return valX.div(valY);
    }

    @Override
    public String signOperation() {
        return "/";
    }
}
