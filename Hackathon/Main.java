import java.io.IOException;
import java.util.TreeMap;

/**
 * Created by pva701 on 3/16/14.
 */
public class Main {
    static final String YOUR_RESULT_FILE_NAME = "output.csv";
    static final String CORRECT_TRAIN_LIKES = "correctTrain.csv";//"correctTrainLikes";///corre..1
    static final int COUNT_POST = 800000;
    static Double[] ans = new Double[COUNT_POST];
    static final String EMPTY_FILE = "EMPTY_";

    public static void main(String[] args) throws IOException {
        Solution sol1 = new Solution(ans);
        sol1.startSolution();
        sol1 = null;

        ReadWriteStreams out = new ReadWriteStreams(EMPTY_FILE, YOUR_RESULT_FILE_NAME);
        for (int i = 0; i < COUNT_POST; ++i)
            if (ans[i] != null)
                out.println(i + " " + ans[i]);
        out.close();
        new CheckerSolution(CORRECT_TRAIN_LIKES, YOUR_RESULT_FILE_NAME);
        System.out.println("checker completed!");
    }
}
