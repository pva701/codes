import java.io.IOException;
import java.util.ArrayList;
import java.util.TreeMap;
import java.util.TreeSet;

/**
 * Created by pva701 on 3/21/14.
 */

public class DictionaryTags {
    static final String WRONG_WORDS = "BITCH";
    static final String EMPTY_ = "EMPTY_";
    static final String DICT = "GoodWords";
    static TreeMap <Integer, String> replaceMap = new TreeMap<Integer, String>();
    static TreeMap <Integer, Integer> numberVert = new TreeMap<Integer, Integer>();

    static ArrayList <String> allMainTags = new ArrayList<String>();

    static public void load() throws IOException {
        ReadWriteStreams wrongWordsStream = new ReadWriteStreams(WRONG_WORDS, EMPTY_);
        TreeSet <String> wrongWordsSet = new TreeSet<String>();
        /*while (!wrongWordsStream.eof()) {
            String curs = wrongWordsStream.next();
            wrongWordsStream.nextInt();
            wrongWordsSet.add(curs);
        }*/
        wrongWordsStream.close();
        wrongWordsSet.add("");

        ReadWriteStreams dictionaryStream = new ReadWriteStreams(DICT, EMPTY_);
        while (!dictionaryStream.eof()) {
            String curLine = dictionaryStream.line();
            String[] arrWord = curLine.split(" ");
            if (arrWord.length < 1 || wrongWordsSet.contains(arrWord[0]))
                continue;
            addMainTag(arrWord[0]);
            for (int j = 0; j < arrWord.length; ++j)
                if (!wrongWordsSet.contains(arrWord[j]) && !arrWord[j].isEmpty())
                    replaceMap.put(arrWord[j].hashCode(), arrWord[0]);
        }
        dictionaryStream.close();
    }

    static boolean contains(String s) {
        return replaceMap.containsKey(s.hashCode());
    }

    static public String getMainTag(String s) {
        if (replaceMap.containsKey(s.hashCode()))
            return replaceMap.get(s.hashCode());
        return null;
    }

    static boolean isMainTag(String s) {
        return contains(s) && replaceMap.get(s.hashCode()).equals(s);
    }

    static public int[] filterSpliting(String s) {
        ArrayList <Integer> spl = new ArrayList<Integer>();
        s = s.toLowerCase();
        int[] words;
        for (int pos = 0; pos < s.length();) {
            String word = "";
            while (pos < s.length() && Character.isAlphabetic(s.charAt(pos)))
                word += s.charAt(pos++);
            if (contains(word))
                spl.add(getNumberTagString(getMainTag(word)));
            ++pos;
        }
        words = new int[spl.size()];
        for (int i = 0; i < words.length; ++i)
            words[i] = spl.get(i);
        return words;
        //return s.toLowerCase().split("[^a-zа-я]");
    }

    static private void addMainTag(String s) {
        if (numberVert.containsKey(s.hashCode()))
            return;

        int sz = numberVert.size();
        numberVert.put(s.hashCode(), sz);
        allMainTags.add(s);
    }


    static public int getNumberTagString(String s) {
        return numberVert.get(s.hashCode());
    }

    static int size() {
        return numberVert.size();
    }
}