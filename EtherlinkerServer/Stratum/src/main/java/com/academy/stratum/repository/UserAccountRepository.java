package com.academy.stratum.repository;

import com.academy.stratum.entity.UserAccount;

public interface UserAccountRepository {

    void persist(UserAccount userAccount);

    void delete(String login);

    UserAccount getByLogin(String login);
}
