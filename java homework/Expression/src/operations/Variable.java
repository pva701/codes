package operations;

import operations.Expression3;
import types.INumber;

/**
 * Created by pva701 on 3/15/14.
 */
public class Variable<T extends INumber<T> > implements Expression3<T> {
    private String name;
    public Variable(String nm) {
        name = nm;
    }

    @Override
    public T evaluate(T x, T y, T z) {
        if (name.equals("x"))
            return x;
        if (name.equals("y"))
            return y;
        return z;
    }
}
