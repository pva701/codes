/**
 * Created by pva701 on 3/16/14.
 */
import java.util.*;
import java.io.*;

class ReadWriteStreams {
    private FastScanner in;
    private PrintWriter out;
    ReadWriteStreams(String inputFileName, String outputFileName) {
        try {
            in = new FastScanner(new File(inputFileName));
            out = new PrintWriter(new File(outputFileName));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private class FastScanner {
        BufferedReader br;
        StringTokenizer st;
        String nextLine = "";
        FastScanner(File f) {
            try {
                br = new BufferedReader(new FileReader(f));
                nextLine = br.readLine();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        String next() throws IOException {
            while (st == null || !st.hasMoreTokens()) {
                try {
                    st = new StringTokenizer(nextLine);
                    nextLine = br.readLine();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return st.nextToken();
        }

        int nextInt() throws IOException {
            return Integer.parseInt(next());
        }

        long nextLong() throws IOException {
            return Long.parseLong(next());
        }

        double nextDouble() throws IOException {
            return Double.parseDouble(next());
        }

        String line() throws IOException {
            if (st == null || !st.hasMoreTokens()) {
                st = new StringTokenizer(nextLine);
                nextLine = br.readLine();
            }

            String str = "";
            boolean frst = true;
            while (st.hasMoreTokens()) {
                if (!frst)
                    str = str.concat(" ");
                str = str.concat(st.nextToken());
                frst = false;
            }
            return str;
        }

        boolean eof() throws IOException {
            if ((st == null || !st.hasMoreTokens()) && nextLine == null)
                return true;
            return false;
        }
    }

    String next() throws IOException{
        return in.next();
    }

    int nextInt() throws IOException{
        return in.nextInt();
    }

    long nextLong() throws IOException{
        return in.nextLong();
    }

    double nextDouble() throws IOException{
        return in.nextDouble();
    }

    void print(Object x) {
        out.print(x);
    }

    void println(Object x) {
        out.println(x);
    }

    void close() {
        out.close();
    }

    String line() throws IOException {
        return in.line();
    }

    boolean eof() throws IOException {
        return in.eof();
    }
}
