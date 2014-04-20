package exceptions;

/**
 * Created by pva701 on 4/14/14.
 */
public class UnpairedBracketsException extends ExpressionParsingException {
    public UnpairedBracketsException(String message) {
        super(message);
    }
    public UnpairedBracketsException(int pos) {
        super(pos);
    }
}
