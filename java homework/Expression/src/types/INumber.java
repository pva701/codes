package types;
import exceptions.*;

/**
 * Created by pva701 on 4/19/14.
 */
public abstract class INumber<T> {
    public abstract T add(T x) throws OverflowArithmeticException;//this + x
    public abstract T sub(T x) throws OverflowArithmeticException;
    public abstract T mul(T x) throws OverflowArithmeticException;
    public abstract T div(T x) throws OverflowArithmeticException, DivisionByZeroArithmeticException;
    public abstract T not(); //~this
    public abstract T unary() throws OverflowArithmeticException;//-this
    public abstract T power(T x) throws OverflowArithmeticException, NegativePowerArithmeticException;//this^x
    public abstract T abs() throws OverflowArithmeticException;
    public abstract T lb() throws LbInvalidArgumentException;
    public abstract T createInstance(String s) throws OverflowArithmeticException, UnexpectedTokenException;
}
