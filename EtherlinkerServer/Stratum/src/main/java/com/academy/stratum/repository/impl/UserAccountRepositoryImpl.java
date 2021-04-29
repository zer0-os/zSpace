package com.academy.stratum.repository.impl;

import com.academy.stratum.entity.UserAccount;
import com.academy.stratum.repository.UserAccountRepository;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DuplicateKeyException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.stereotype.Repository;

import java.sql.ResultSet;
import java.sql.Types;
import java.util.List;
import java.util.UUID;

@Repository("userAccountRepository")
public class UserAccountRepositoryImpl implements UserAccountRepository {

    private static final Logger LOG = LoggerFactory.getLogger(UserAccountRepositoryImpl.class);

    @Autowired
    protected JdbcTemplate jdbcTemplate;

    public static final RowMapper<UserAccount> ROW_MAPPER = (ResultSet rs, int rowNum) ->
            new UserAccount(
                    UUID.fromString(rs.getString("user_account_id")),
                    rs.getString("login"),
                    rs.getString("password"),
                    rs.getBytes("encryption_keys"),
                    rs.getBytes("wallet_data")
            );

    @Override
    public void persist(UserAccount userAccount) {
        Object[] params = new Object[]{userAccount.getUserAccountId(), userAccount.getLogin(), userAccount.getPassword(), userAccount.getEncryptionKeys(), userAccount.getWalletData()};
        int[] types = new int[]{Types.VARCHAR, Types.VARCHAR, Types.VARCHAR, Types.BINARY, Types.BINARY};

        try {
            jdbcTemplate.update("INSERT INTO user_accounts(\n" +
                    "            user_account_id, login, password, encryption_keys, wallet_data, date_create)\n" +
                    "    VALUES (cast(? as UUID), ?, ?, ?, ?, current_timestamp);", params, types);
        } catch (DuplicateKeyException e) {
            throw new RuntimeException("User with this login already exist");
        }
    }

    @Override
    public void delete(String login) {
        Object[] params = new Object[]{login};
        int[] types = new int[]{Types.VARCHAR};
        jdbcTemplate.update("DELETE FROM user_accounts\n" +
                " WHERE login = ?", params, types);
    }

    @Override
    public UserAccount getByLogin(String login) {
        Object[] params = new Object[]{login};
        int[] types = new int[]{Types.VARCHAR};
        List<UserAccount> userAccountList = jdbcTemplate.query("SELECT user_account_id, login, password, encryption_keys, wallet_data FROM user_accounts WHERE login = ?;", params, types, ROW_MAPPER);
        if(!userAccountList.isEmpty()) {
            return userAccountList.get(0);
        } else return null;
    }
}
