package operations;
import types.INumber;

/**
 * Created by pva701 on 4/1/14.
 */
public class Abs<T extends INumber<T> > implements Expression3<T> {
    Expression3<T> a;
    public Abs(Expression3<T> a) {
        this.a = a;
    }

    @Override
    public T evaluate(T x, T y, T z) {
        return  a.evaluate(x, y, z).abs();
    }
}
