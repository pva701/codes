import java.io.IOException;
import java.sql.Date;
import java.util.ArrayList;
import java.util.TreeMap;

/**
 * Created by pva701 on 3/17/14.
 */
class Post {
    Post(int sid, int gid, Community cm, int stmp, String cnts) {
        selfId = sid;
        groupId = gid;
        parentCommunity = cm;
        timeStamp = stmp;
        content = DictionaryTags.filterSpliting(cnts);
    }

    protected int selfId, groupId, likes, mask;
    protected Community parentCommunity;
    protected int timeStamp;
    protected int[] content;///string
    protected void deleteContent() {
        content = null;
    }


    //private ArrayList <User> likeUser;
    //public int getContent() {return content;}
    public int getId() {
        return selfId;
    }

    public int getTimeStamp() {
        return timeStamp;
    }

    public void addLike() {
        likes++;
    }
    public int getLikes() {
        return likes;
    }

    public Community getParentCommunity() {
        return parentCommunity;
    }

    public void setLikes(int x) {
        likes = x;
    }

    void setMask(int x) {
        mask = x;
    }

    int getMask() {
        return mask;
    }
}
