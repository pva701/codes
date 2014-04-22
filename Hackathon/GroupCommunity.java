import java.io.IOException;
import java.util.*;

/**
 * Created by pva701 on 3/18/14.
 */
class GroupCommunity {
    static protected final int COUNT_COMMUNITY = 4600;
    static protected final int NUMBER_TAGS = Community.NUMBER_TAGS;
    static protected final String EMPTY_ = "EMPTY_";
    static protected Community[] comm;

    GroupCommunity() throws IOException {

    }

    static public void addPost(int numberCom, Post x) {
        comm[numberCom].addPost(x);
    }

    static public int sizeCommunities() {
        return COUNT_COMMUNITY;
    }

    static public Community getCommunityById(int id) {
        return comm[id];
    }

    static MarkedArray mar;
    static public ArrayList<Integer> sortTags(ArrayList <Integer> tags, Community curCom) {
        final int SHIFT = DictionaryTags.size();
        int sz = NUMBER_TAGS; //sz = min(MAX_NUMBER_TAGS, COEF_FOR_LIKES * user.getLikes());
        TreeSet <Integer> srt = new TreeSet<Integer>();
        int id = curCom.getId();
        mar.clear();
        for (int i = 0; i < tags.size(); ++i)
            mar.add(tags.get(i), id);

        for (int i = 0; i < mar.sizeTouched; ++i) {
            int addValue = -(mar.sum[mar.touched[i]] * SHIFT + mar.touched[i]);
            /*if (mar.sum[mar.touched[i]] >= SHIFT)
                throw new RuntimeException();*/
            srt.add(addValue);
            if (srt.size() > sz)
                srt.remove(srt.last());
        }
        sz = min(sz, srt.size());
        Iterator <Integer> it = srt.iterator();
        ArrayList <Integer> ans = new ArrayList<Integer>();
        for (int i = 0; i < sz; ++i) {
            int cur = -it.next();
            ans.add(cur % SHIFT);
        }
        return ans;
    }
    static Comparator <PairIntInt> compForMasks = new Comparator<PairIntInt>() {
        @Override
        public int compare(PairIntInt o1, PairIntInt o2) {
            if (o1.first == o2.first)
                return o1.second - o2.second;
            return o1.first - o2.first;
        }
    };

    private static void buildTags() {
        int szDic = DictionaryTags.size();
        mar = new MarkedArray(szDic);
        int tot = 1<<NUMBER_TAGS;
        for (int i = 1; i < COUNT_COMMUNITY; ++i) {
            ArrayList <Integer> curWords = new ArrayList<Integer>();
            ArrayList <PairIntInt> countWord = new ArrayList<PairIntInt>();
            ///sorted posts
            //Collections.sort(comm[i].posts, compForMasks);

            for (int j = 0; j < comm[i].posts.size(); ++j) {
                comm[i].avlikes += comm[i].posts.get(j).getLikes();
                int[] curPostText = comm[i].posts.get(j).content;
                for (int k = 0; k < curPostText.length; ++k)
                    curWords.add(curPostText[k]);
            }
            ArrayList <Integer> resTag = sortTags(curWords, comm[i]);
            /*if (resTag.size() > 1) resTag.remove(resTag.size() - 1);
            if (resTag.size() > 1) resTag.remove(resTag.size() - 1);
            resTag.add(DictionaryTags.getNumberTagString("?"));
            resTag.add(DictionaryTags.getNumberTagString("!"));*/

            for (int j = 0; j < resTag.size(); ++j)
                comm[i].addTag(resTag.get(j));

            int[] tmpCnt = new int[1<<NUMBER_TAGS];
            for (int j = 0; j < comm[i].posts.size(); ++j) {
                int msk = 0;
                boolean[] wasTAG = new boolean[NUMBER_TAGS];
                int[] curPostText = comm[i].posts.get(j).content;
                for (int k = 0; k < curPostText.length; ++k) {
                    if (!comm[i].tagContain(curPostText[k]))
                        continue;
                    Integer id = comm[i].getIdTag(curPostText[k]);
                    if (!wasTAG[id])
                        msk |= 1<<id;
                    wasTAG[id] = true;
                }
                tmpCnt[msk]++;
                comm[i].scoreForMask[msk] += comm[i].posts.get(j).getLikes();
                comm[i].posts.get(j).setMask(msk);
                //comm[i].posts.get(j).deleteContent();
                //comm[i].posts.set(j, null);
            }

            if (comm[i].posts.size() > 0)
                comm[i].avlikes /= comm[i].posts.size();

            for (int j = 0; j < tot; ++j)
                if (tmpCnt[j] > 0) {
                    comm[i].scoreForMask[j] /= tmpCnt[j];
                    comm[i].know[j] = true;
                }

            if (i % 500 == 0) System.out.println(i);
        }
    }

    private static  void countWords() {
        int q = 0;
        int[] mark = new int[DictionaryTags.size()];
        for (int i = 1; i < COUNT_COMMUNITY; ++i) {
            for (int j = 0; j < comm[i].posts.size(); ++j) {
                ++q;
                int[] curPostText = comm[i].posts.get(j).content;
                for (int k = 0; k < curPostText.length; ++k)
                    if (mark[curPostText[k]] != q) {
                        comm[i].addWord(curPostText[k], comm[i].posts.get(j).getLikes());
                        //comm[i].addWordNegative(curPostText[k], (int)(comm[i].posts.get(j).getLikes() - comm[i].avlikes));
                        mark[curPostText[k]] = q;
                    }
            }
            if (i % 500 == 0) System.out.println(i);
        }
    }

    static final boolean READ_FROM_FILE = false;
    static void arrange() throws IOException {
        if (READ_FROM_FILE)
            readFromFile();
        if (!READ_FROM_FILE) {
            buildTags();
            countWords();

            int bit = 1395;
            int[] cnt = new int[7];
            int[] sum = new int[7];
            for (int i = 0; i < comm[bit].posts.size(); ++i) {
                Date cur = new Date(1L * comm[bit].posts.get(i).getTimeStamp() * 1000);
                cnt[cur.getDay()]++;
                sum[cur.getDay()] += comm[bit].posts.get(i).getLikes();
            }

            for (int j = 0; j < 7; ++j)
                System.out.println(1.0 * sum[j] / cnt[j]);
        }
        System.out.println("tags was added in groups!");
    }

    static private void readFromFile() throws IOException {
        ReadWriteStreams in = new ReadWriteStreams("tagsComm", EMPTY_);
        while (!in.eof()) {
            int idGroup = in.nextInt();
            int av = in.nextInt();
            int cnt = in.nextInt();
            comm[idGroup].setAvLikes(av);
            for (int j = 0; j < cnt; ++j) {
                int tagj = in.nextInt();
                int x = in.nextInt();
                int y = in.nextInt();
                //comm[idGroup].setCountSumTag(tagj, x, y);
            }
        }
        in.close();
    }

    static private int min(int a, int b) {
        if (a < b)
            return a;
        return b;
    }

    static private int abs(int x) {
        if (x > 0)
            return x;
        return -x;
    }
    //public Community getCommunityById(Integer id) {return comm[id];}
}
