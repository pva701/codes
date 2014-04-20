package operations;

import exceptions.OverflowArithmeticException;
import operations.Expression3;
import types.INumber;

/**
 * Created by pva701 on 4/1/14.
 */
public class Not<T extends INumber<T> > implements Expression3<T> {
    Expression3<T> a;
    public Not(Expression3<T> a) {
        this.a = a;
    }

    @Override
    public T evaluate(T x, T y, T z) throws OverflowArithmeticException {
        return a.evaluate(x, y, z).not();
    }
}
