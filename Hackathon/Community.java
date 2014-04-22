import java.util.*;

/**
 * Created by pva701 on 3/17/14.
 */
class Community {
    protected int selfId;
    static protected final int NUMBER_TAGS = 10;
    protected ArrayList <Post> posts = new ArrayList<Post>();
    protected TreeMap <Integer, Integer> tags = new TreeMap<Integer, Integer>();
    protected TreeMap <Integer, Integer> countLikesWord = new TreeMap <Integer, Integer>();
    protected TreeMap <Integer, Integer> sumLikesWord = new TreeMap <Integer, Integer>();
    //protected TreeMap <Integer, Integer> negCountLikesWord = new TreeMap<Integer, Integer>();
    //protected TreeMap <Integer, Integer> negSumLikesWord = new TreeMap<Integer, Integer>();
    protected ArrayList <Integer> listTags = new ArrayList<Integer>();
    protected double avlikes;
    protected int maxFreq;
    protected int negMaxFreq;
    //protected TreeMap <Integer, Integer> whereWord = new TreeMap<Integer, Integer>();
    //ArrayList <ArrayList<Integer> > wordsPost = new ArrayList<ArrayList<Integer> >();
    protected double[] scoreForMask = new double[1<<NUMBER_TAGS];
    protected boolean[] know = new boolean[1<<NUMBER_TAGS];
    //protected ArrayList<Post>[] postByMask = new ArrayList[1<<NUMBER_TAGS];

    Community() {}
    Community(int id) {
        selfId = id;
    }

    public double getAvLikesForEmptyPosts() {
        return avlikes;
    }

    public ArrayList<Post> getPostsList() {
        return posts;
    }

    public boolean containWord(int id) {
        return sumLikesWord.containsKey(id);
    }
    public int getIdBasket(int id) {
        return sumLikesWord.get(id);
    }

    public void addWord(int idWord, int likes) {
        if (!containWord(idWord)) {
            //wordsPost.add(new ArrayList<Integer>());
            //whereWord.put(idWord, wordsPost.size() - 1);
            countLikesWord.put(idWord, 0);
            sumLikesWord.put(idWord, 0);
        }
        //wordsPost.get(whereWord.get(idWord)).add(idPost);
        countLikesWord.put(idWord, countLikesWord.get(idWord) + 1);
        if (countLikesWord.get(idWord) > maxFreq)
            maxFreq = countLikesWord.get(idWord);
        sumLikesWord.put(idWord, sumLikesWord.get(idWord) + likes);
    }

    /*public void addWordNegative(int idWord, int likes) {
        if (likes >= 0) return;

        if (!negCountLikesWord.containsKey(idWord)) {
            negCountLikesWord.put(idWord, 0);
            negSumLikesWord.put(idWord, 0);
        }
        negCountLikesWord.put(idWord, countLikesWord.get(idWord) + 1);
        if (negCountLikesWord.get(idWord) > negMaxFreq)
            negMaxFreq = negCountLikesWord.get(idWord);
        negSumLikesWord.put(idWord, negSumLikesWord.get(idWord) + likes);
    }*/

    public Post getPost(int i) {
        return posts.get(i);
    }
    public void addPost(Post x) {
        posts.add(x);
    }

    public Post getPostById(int postId) {//slow
        for (int i = 0; i < posts.size(); ++i)
            if (posts.get(i).getId() == postId)
                return posts.get(i);
        throw new RuntimeException();
    }

    public int sizePosts() {
        return posts.size();
    }
    public int getId() {
        return selfId;
    }
    public void setAvLikes(int x) {
        avlikes = x;
    }

    int getNumberTags() {
        return tags.size();
    }

    public void addTag(int s) {
        int sz = tags.size();
        tags.put(s, sz);
        listTags.add(s);
    }

    public boolean tagContain(int s) {
        return tags.containsKey(s);
    }

    public int getIdTag(int s) {
        return tags.get(s);
    }

    /*public void addLikeForTag(int id, int addl) {
        sumLikesTag.set(id, sumLikesTag.get(id) + addl);
        countLikesTag.set(id, countLikesTag.get(id) + 1);
    }*/

    public int getAverageLikes(int id) {
        return sumLikesWord.get(id) / countLikesWord.get(id);
    }

    public int getNegAverageLikes(int id) {
        return sumLikesWord.get(id) / countLikesWord.get(id);
    }
}
