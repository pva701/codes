import java.io.IOException;
import java.sql.Date;
import java.util.ArrayList;
import java.util.TreeMap;

/**
 * Created by pva701 on 3/22/14.
 */
public class DateTime {
    static public void initDates() throws IOException {
        final ArrayList<Double> cntPgViews = new ArrayList<Double>();
        final ArrayList <Date> dtPgViews = new ArrayList<Date>();
        TreeMap<String, Integer> key = new TreeMap<String, Integer>();
        key.put("Jan", 1);
        key.put("Feb", 2);
        key.put("Mar", 3);
        key.put("Apr", 4);
        key.put("May", 5);
        key.put("Jun", 6);
        key.put("Jul", 7);
        key.put("Aug", 8);
        key.put("Sep", 9);
        key.put("Oct", 10);
        key.put("Nov", 11);
        key.put("Dec", 12);
        ReadWriteStreams in = new ReadWriteStreams("Activity_by_Date", "EMPTY_");
        while (!in.eof()) {
            String month = in.next();
            int year = in.nextInt();
            month = month.substring(0, month.length() - 1);
            String cur = "20".concat(Integer.toString(year)).concat("-").concat(Integer.toString(key.get(month))).concat("-01");
            dtPgViews.add(Date.valueOf(cur));
        }
        //cntPgViews.add()
    }

    static public double getCoef(long mills) {
        final long MAX_PAGEVIEWS = 50000000000L;
        java.util.Date curDate = new java.util.Date(mills);
        /*int i = 0;
        while (curDate.compareTo(dtPgViews.get(i)) == 1)
            ++i;r
        //double coef = 1.0 * cntPgViews.get(i) / MAX_PAGEVIEWS;*/
        double coef = 1.0;
        //if (curDate.getDay() == 0 || curDate.getDay() == 6) coef *= 0.7;
        //if (curDate.getHours() >= 20 && curDate.getHours() <= 22) coef /=
        int mon = curDate.getMonth();

        if (2 <= mon) {
            double a = 1.0 / 3;
            double b = -13.0 / 3;
            double mn = b * b / (4 * a * a) - b * b / (2 * a);
            double mx = 4 * a + b * 2;
            double A = 0.5 / (mx - mn);
            double B = 1 - A * mx;
            double f = a * mon * mon + b * mon;
            coef *= A * f + B;
            /////258
        }
        /*mon -= 2;
        if (mon < 0) mon += 12;
        if (mon <= 9) {
            double div = 2 - Math.abs(4.5 - mon) / 4.5;
            coef /= div;
            ///264
        }*/



        /*if (2 <= mon && mon <= 11) {
            double a = Math.PI / 5.5;
            double b = Math.PI / 2 - 2 * a;
            double f = 0.25 * Math.sin(a * mon + b) + 0.75;
            coef *= f;
        }*/

        ///witout = 268 in easy vers
        return coef;
        /*
        other version - parabol
         */
    }

}
