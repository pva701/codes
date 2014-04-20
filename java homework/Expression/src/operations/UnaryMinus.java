package operations;

import exceptions.OverflowArithmeticException;
import operations.Expression3;
import types.INumber;

/**
 * Created by pva701 on 3/18/14.
 */

public class UnaryMinus<T extends INumber<T> > implements Expression3<T> {
    Expression3<T> a;
    public UnaryMinus(Expression3<T> aa) {
        a = aa;
    }

    @Override
    public T evaluate(T x, T y, T z) throws OverflowArithmeticException {
        return a.evaluate(x, y, z).unary();
    }
}
