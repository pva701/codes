package operations;

import exceptions.OverflowArithmeticException;
import types.INumber;

/**
 * Created by pva701 on 3/15/14.
 */
public class Add<T extends INumber<T> > extends BinaryOperation<T> {
    public Add(Expression3<T> xx, Expression3<T> yy) {
        super(xx, yy);
    }

    @Override
    T op(T valX, T valY) throws OverflowArithmeticException {
        return valX.add(valY);
    }
    @Override
    public String signOperation() {
        return "+";
    }
}
