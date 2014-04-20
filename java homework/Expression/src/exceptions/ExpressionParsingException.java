package exceptions;

/**
 * Created by pva701 on 4/14/14.
 */
public abstract class ExpressionParsingException extends Exception {
    private int position;

    public ExpressionParsingException(int pos) {
        position = pos;
    }

    public ExpressionParsingException(String message) {
        super(message);
    }

    public ExpressionParsingException(String message, int pos) {
        super(message);
        position = pos;
    }

    public int getPosition() {
        return position;
    }
}
