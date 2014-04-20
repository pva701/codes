package exceptions;

/**
 * Created by pva701 on 4/14/14.
 */
public class UnexpectedEndOfExpressionException extends ExpressionParsingException {
    public UnexpectedEndOfExpressionException(String message) {
        super(message);
    }
    public UnexpectedEndOfExpressionException(int pos) {
        super(pos);
    }
}
