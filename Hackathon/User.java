import sun.reflect.generics.tree.Tree;

import java.util.ArrayList;
import java.util.TreeMap;

/**
 * Created by pva701 on 3/17/14.
 */
class User {
    private int selfId;
    private int mxLikes;
    private ArrayList <Integer> comLikes = new ArrayList<Integer>();
    private ArrayList <Integer> cntLikes = new ArrayList<Integer>();
    User(int id) {
        selfId = id;
    }

    /*public void addLike(int x, int y) {
        comLikes.add(x);
        cntLikes.add(y);
    }*/


    /*public void addCommunity(int numCom, int countLikes) {
        //comLikes.add(numCom);
        //cntLikes.add(countLikes);
        if (countLikes > mxLikes)
            mxLikes = countLikes;
    }*/

    void setMxLikes(int mx) {
        if (mx > mxLikes)
            mxLikes = mx;
    }


    public int getMaxLikes() {
        return mxLikes;
    }

    /*public Post getPostById(int postId) {
        for (int i = 0; i < likePosts.size(); ++i)
            if (likePosts.get(i).getId() == postId)
                return likePosts.get(i);
        assert false : "getPostById assertion failed in User!";
        return null;
    }*/

    int getId() {
        return selfId;
    }
}
