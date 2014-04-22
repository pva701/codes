/**
 * Created by pva701 on 3/18/14.
 */
public class GroupPost {///sheet?
    static final int TOTAL_POSTS = 800000;
    static final private Post[] posts = new Post[TOTAL_POSTS];

    GroupPost() {
    }
    static public void setPost(int id, Post x) {
        posts[id] = x;
    }

    static public int size() {
        return TOTAL_POSTS;
    }

    static public Post getPostById(int id) {
        return posts[id];
    }
}
