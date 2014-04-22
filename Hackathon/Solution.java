import java.io.IOException;
import java.util.*;

/**
 * Created by pva701 on 3/17/14.
 */

class Solution {
    /*private final int TOT_USERS = 10400000;
    final int TOT_COMMS = 4600;
    int[] mxLikes = new int[TOT_USERS];
    double[] mathEx = new double[TOT_COMMS];
    double[] avLenWordPost = new double[TOT_USERS];
    double[] avCountImagePost = new double[TOT_USERS];*/

    //private GroupCommunity allCommunities = new GroupCommunity();
    //private GroupPost allPosts = new GroupPost();
    //private GroupUser allUsers = new GroupUser();
    final int TOT_POSTS = 800000;
    final int MILLS = 1000;
    final String EMPTY_FILE = "EMPTY_";
    final String TRAIN_CONTENT_FILE = "train_content.csv";////train_content //2
    final String TEST_CONTENT_FILE =  "test_content.csv";//////test_content //1
    final String POSTS_LIKES = "correctTrain.csv";///correctTrain.csv //2.csv

    Double[] ans;
    Solution(Double[] outp) throws IOException {
        ans = outp;
    }

    private void readTrainContent() throws IOException {
        ReadWriteStreams trainContentStream = new ReadWriteStreams(TRAIN_CONTENT_FILE, EMPTY_FILE);//utf-8 кодировка
        int line = 0;
        while (!trainContentStream.eof()) {
            int groupId = trainContentStream.nextInt();
            int postId = trainContentStream.nextInt();
            int timeStamp = (int)(trainContentStream.nextLong() / MILLS);
            String content = trainContentStream.line();
            Community currentCommunity = GroupCommunity.getCommunityById(groupId);
            Post currentPost = new Post(postId, groupId, currentCommunity, timeStamp, content);
            GroupPost.setPost(postId, currentPost);
            GroupCommunity.addPost(groupId, currentPost);

            ++line;
            if (line % 10000 == 0)
                System.out.println(line);
        }
        trainContentStream.close();
        System.out.println("readTrainContent completed!");
    }

    private void readTrainLikes() throws IOException {
        ReadWriteStreams trainLikeStream = new ReadWriteStreams(POSTS_LIKES, EMPTY_FILE);
        /*while (!trainLikeStream.eof()) {
            int us = trainLikeStream.nextInt();
            trainLikeStream.nextInt();
            trainLikeStream.nextLong();
            cnt[us]++;
        }*/
        while (!trainLikeStream.eof()) {
            int postId = trainLikeStream.nextInt();
            int lks = trainLikeStream.nextInt();
            GroupPost.getPostById(postId).setLikes(lks);
        }
        trainLikeStream.close();
        System.out.println("readTrainLikes completed!");

        GroupCommunity.arrange();
        System.out.println("found tagWord completed!");
    }

    private void readTestContent() throws IOException {
        final int NUMBER_CONCIDENCES = 20000;
        MarkedArray cnt = new MarkedArray(500000);
        ReadWriteStreams testContentStream = new ReadWriteStreams(TEST_CONTENT_FILE, EMPTY_FILE);
        int line = 0, countnull = 0, know = 0, quer = 0, wrong = 0, ug = 0, ugwr = 0;
        double smDiff = 0, maxDiff = 0, minDiff = 0;
        while (!testContentStream.eof()) {
            int groupId = testContentStream.nextInt();
            int postId = testContentStream.nextInt();
            //int timeStamp = (int)(testContentStream.nextLong() / 1000);////LONG
            long timeStamp = testContentStream.nextLong();
            String content = testContentStream.line();
            ++line;
            if (line % 10000 == 0)
                System.out.println(line);

            /*if (GroupPost.getPostById(postId) != null) {
                know++;
                ans[postId] = (double)GroupPost.getPostById(postId).getLikes();
                continue;
            }*/


            Community curCom = GroupCommunity.getCommunityById(groupId);
            if (curCom == null)
                continue;
            ++quer;
            int[] curContent = DictionaryTags.filterSpliting(content);
            int mx = 0, ansIdPost = -1, difflen = 1000000000;
            boolean[] wasId = new boolean[GroupCommunity.getCommunityById(groupId).getNumberTags()];
            int msk = 0;
            double sumLikes = 0, diff = 0;
            double negSumLikes = 0, negDiff = 0;

            /*for (int j = 0; j < curContent.length; ++j) {
                int curWord = curContent[j];
                if (curCom.containWord(curWord)) {
                    sumLikes += 1.0 * curCom.getAverageLikes(curWord) * curCom.countLikesWord.get(curWord) / curCom.maxFreq;
                    diff += 1.0 * curCom.countLikesWord.get(curWord) / curCom.maxFreq;

                    /*if (curCom.negCountLikesWord.containsKey(curWord)) {
                        negSumLikes += curCom.getNegAverageLikes(curWord) * curCom.negCountLikesWord.get(curWord) / curCom.negMaxFreq;
                        negDiff += 1.0 * curCom.negCountLikesWord.get(curWord) / curCom.negMaxFreq;
                    }*

                }
            }*/

            for (int j = 0; j < curContent.length; ++j) {
                int curWord = curContent[j];
                if (curCom.tagContain(curWord)) {
                    int id = curCom.getIdTag(curWord);
                    if (!wasId[id])
                        msk |= 1<<id;
                    wasId[id] = true;
                    }
            }

            if (msk == 0)
                ++wrong;
            ans[postId] = 0.0;
            /*if (ansIdPost != -1 && sm == 1) {
                ug++;
                if (msk == 0)
                    ugwr++;
                ans[postId] = 1.0 * alllikes / sm; //1.0 * curCom.posts.get(ansIdPost).getLikes();
                //if (curCom.posts.get(ansIdPost).content.length != curContent.length) {
                    /*System.out.println(mx);
                    System.out.println(sm);
                    System.out.println("content = " + content);
                    System.out.println("prog ver = " + curCom.posts.get(ansIdPost).tmpCont);*

            } else*/

            ans[postId] = curCom.scoreForMask[msk];
            //ans[postId] = curCom.getAvLikesForEmptyPosts();
            //else ans[postId] = curCom.scoreForMask[msk];
            //if (isHoliday(content)) ans[postId] *= 2;
            //ans[postId] *= DateTime.getCoef(timeStamp);//curCom.getLikesForTime(msk, timeStamp);//260
            if (ans[postId] < 0.1)
                countnull++;
            ans[postId] = Math.max(ans[postId], 0.0);
        }
        System.out.println("readTestContent completed! with nill posts " + countnull + " and with knowed posts = " + know);
        System.out.println("wrong = " + wrong);
        System.out.println("ug = " + ug);
        System.out.println("ugwr = " + ugwr);

        System.out.println(minDiff + " " + maxDiff + " av = " + 1.0 * smDiff / line);

            /*minDiff  = Math.min(minDiff, ans[postId] - curCom.scoreForMask[msk]);
                maxDiff  = Math.max(maxDiff, ans[postId] - curCom.scoreForMask[msk]);
                smDiff += ans[postId] - curCom.scoreForMask[msk];*/

                            /*int cnttags = 0;
            for (int j = 0; j < curContent.length; ++j) {
                int curWord = curContent[j];
                if (curCom.tagContain(curWord)) {
                    int id = curCom.getIdTag(curWord);
                    if (!wasId[id]) {
                        cnttags++;
                        msk |= 1<<id;
                    }
                    wasId[id] = true;
                }
            }*/
            //System.out.println(Integer.bitCount(msk));

            /*int alllikes = 0, sm = 0;
            for (int j = 0; j < curContent.length; ++j) {
                int curWord = curContent[j];
                if (curCom.containWord(curWord)) {
                    int idBasket = curCom.getIdBasket(curWord);
                    ArrayList <Integer> curBasket = curCom.wordsPost.get(idBasket);

                    for (int k = 0; k < curBasket.size(); ++k) {
                        int curPost = curBasket.get(k);
                        if (curCom.posts.get(curPost).getMask() == msk) {
                            cnt.add(curPost, quer);
                            int len = curCom.posts.get(curPost).content.length;
                            if (cnt.sum[curPost] > mx || cnt.sum[curPost] == mx && abs(len - curContent.length) < difflen) {
                                mx = cnt.sum[curPost];
                                ansIdPost = curPost;
                                alllikes = curCom.posts.get(curPost).getLikes();
                                difflen = len;
                                sm = 1;
                            }  else if (cnt.sum[curPost] == mx) {
                                alllikes += curCom.posts.get(curPost).getLikes();
                                ++sm;
                            }
                        }
                    }
                }
                cnt.clear();
            }*/
    }
    void startSolution() throws IOException {
        //DateTime.initDates();
        GroupCommunity.comm = new Community[GroupCommunity.COUNT_COMMUNITY];
        for (int i = 0; i < GroupCommunity.COUNT_COMMUNITY; ++i)
            GroupCommunity.comm[i] = new Community(i);

        DictionaryTags.load();
        readTrainContent();
        readTrainLikes();
        readTestContent();
    }

    static boolean isHoliday(String s) {
        String[] keys = {"праздник", "праздником", "праздника", "праздника", "о празднике", "с днем", "с днём", "с этим днём", "защитника отечества",
                "поздравлять", "поздравить", "поздравляю", "поздравление", "поздравим", "поздсравите", "поздравлю"};
        TreeSet <String> good = new TreeSet<String>();
        for (int i = 0; i < keys.length; ++i)
            good.add(keys[i]);
        String[] ar = s.toLowerCase().split("[^а-яa-z]");
        for (int i = 0; i < ar.length; ++i)
            if (good.contains(ar[i]))
                return true;
        return false;
    }

    private int abs(int x) {
        if (x < 0)
            return -x;
        return x;
    }

    private double sqr(double x) {
        return x * x;
    }
}
