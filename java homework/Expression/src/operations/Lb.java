package operations;

import exceptions.LbInvalidArgumentException;
import types.INumber;

/**
 * Created by pva701 on 4/15/14.
 */
public class Lb<T extends INumber<T> > implements Expression3<T> {
    private  Expression3<T> a;
    public Lb(Expression3<T> a) {
        this.a = a;
    }

    @Override
    public T evaluate(T x, T y, T z) throws LbInvalidArgumentException {
        return a.evaluate(x, y, z).lb();
    }
}
