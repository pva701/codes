/**
 * Created by pva701 on 3/18/14.
 */
public class GroupUser {
    private final int TOT_USERS = 10400000;///TreeMap
    private User[] users = new User[TOT_USERS];
    //private int[] users = new int[TOT_USERS];

    public User getUserById(int id) {
        if (users[id] == null)
            users[id] = new User(id);
        //users[id]++;
        return users[id];
    }
}
