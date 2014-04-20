package operations;

import types.INumber;

/**
 * Created by pva701 on 3/15/14.
 */

public class Const<T extends INumber<T> > implements Expression3 <T> {
    T val;
    public Const(T xx) {
        val = xx;
    }

    @Override
    public T evaluate(T x, T y, T z){
        return val;
    }
}
