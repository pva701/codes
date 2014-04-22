import java.io.IOException;

/**
 * Created by pva701 on 3/17/14.
 */
public class CheckerSolution {
    final String VERDICT_FILE_NAME = "verdict.txt";
    final String EMPTY_FILE = "EMPTY_";
    final ReadWriteStreams correctResultsStream;
    final ReadWriteStreams yourResultsStream;
    final int NUMBER_POSTS = 800000;
    final int[] likes = new int[NUMBER_POSTS];
    final boolean[] wasPost = new boolean[NUMBER_POSTS];

    CheckerSolution(String correctResultsFileName, String yourResultsFileName) throws IOException{
        correctResultsStream = new ReadWriteStreams(correctResultsFileName, VERDICT_FILE_NAME);
        yourResultsStream = new ReadWriteStreams(yourResultsFileName, EMPTY_FILE);

        int countPosts = 0, totalLikes = 0;
        while (!correctResultsStream.eof()) {
            countPosts++;
            int numPost = correctResultsStream.nextInt();
            likes[numPost] = correctResultsStream.nextInt();
            totalLikes += likes[numPost];
        }

        int yourCountPosts = 0, differenceLike = 0;
        double ex1 = 0, ex2 = 0, ex3 = 0, ex4 = 0, d = 0;
        int cc1 = 0, cc2 = 0;
        while (!yourResultsStream.eof()) {
            int id = yourResultsStream.nextInt();
            double yourLikes = yourResultsStream.nextDouble();
            if (Double.isNaN(yourLikes) || Double.isInfinite(yourLikes) || yourLikes < 0)
                throw new RuntimeException();

            ++yourCountPosts;
            if (wasPost[id])
                throw new RuntimeException();
            wasPost[id] = true;
            differenceLike += abs(likes[id] - yourLikes);
            double diff = likes[id] - yourLikes;
            ex1 += 1.0 * diff * diff;
            ex2 += diff;
            ex3 += 1.0 * likes[id] * likes[id];
            ex4 += likes[id];

            d += likes[id] - yourLikes;
            if (yourLikes < 0.1)
                cc1 += abs(likes[id] - yourLikes);
            cc2 += abs(likes[id] - yourLikes);

        }
        System.out.println(cc1 + " " + cc2);
        if (yourCountPosts != countPosts)
            throw new RuntimeException();
        correctResultsStream.println(1.0 * differenceLike / totalLikes);
        ex1 /= countPosts;
        ex2 /= countPosts;
        ex3 /= countPosts;
        ex4 /= countPosts;
        correctResultsStream.println((1 - (ex1 - ex2 * ex2) / (ex3 - ex4 * ex4)) * 1000);
        correctResultsStream.println(d);

        correctResultsStream.close();
        yourResultsStream.close();
    }

    private double abs(double x) {
        if (x < 0)
            return -x;
        return x;
    }
}
