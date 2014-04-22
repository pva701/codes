/**
 * Created by pva701 on 3/22/14.
 */
public class MarkedArray {
    int[] touched;
    int[] marked;
    int[] sum;
    int size, sizeTouched;
    MarkedArray(int size) {
        touched = new int[size];
        marked = new int[size];
        sum = new int[size];
        this.size = size;
        sizeTouched = 0;
    }

    void add(int i, int mark) {
        if (marked[i] != mark) {
            marked[i] = mark;
            sum[i] = 0;
            touched[sizeTouched++] = i;
        }
        sum[i]++;
    }

    void clear() {
        sizeTouched = 0;
    }
}