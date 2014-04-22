/**
 * Created by pva701 on 3/26/14.
 */
public class MarkedArrayDouble {
    int[] marked;
    double[] sum;
    int size;
    MarkedArrayDouble(int size) {
        marked = new int[size];
        sum = new double[size];
        this.size = size;
    }

    void add(int i, int mark, double val) {
        if (marked[i] != mark) {
            marked[i] = mark;
            sum[i] = 0;
        }
        sum[i] += val;
    }
}
