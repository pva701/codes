package exceptions;

/**
 * Created by pva701 on 4/14/14.
 */
public class UnexpectedTokenException extends ExpressionParsingException {
    public UnexpectedTokenException(String message) {
        super(message);
    }
    public UnexpectedTokenException(String message, int pos) {
        super(message, pos);
    }
}
