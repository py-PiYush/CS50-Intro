-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Read description of the crime for more detatils
SELECT description
FROM crime_scene_reports
WHERE year = 2021
AND month = 7
AND day = 28
AND street = "Humphrey Street";

-- Description of crime mentioned 3 witnesses, let's read their interviews
-- Also time of crime = 10:15am
SELECT name, transcript
FROM interviews
WHERE year = 2021
AND month = 7
AND day = 28
AND transcript LIKE "%bakery%";

-- Ruth - after 10 minutes thief left bakery parking lot in car. look security footage
-- Eugene - bakery = Emma's bakery, ATM on Leggett Street, theif withdrawing money
-- Raymond - talking to someone for less than a minute, earliest flight out of Fiftyville on July 29 2021. Person on other end purchased tickets

-- Let's find out who withdrew money on morning of July 28, 2021 and also was seen in security footage
-- +-------+
-- | name  |
-- +-------+
-- | Bruce |
-- | Diana |
-- | Iman  |
-- | Luca  |
-- +-------+
-- Then filter out based on who called that day for less than 60 seconds
-- +-------+-----------------+
-- | name  | passport_number |
-- +-------+-----------------+
-- | Diana | 3592750733      |
-- | Bruce | 5773159633      |
-- +-------+-----------------+
SELECT name, passport_number
FROM people
WHERE phone_number IN (
    -- called for < 60s
    SELECT caller
    FROM phone_calls
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60
    AND caller IN(
        -- Withdrew money on theft day
        SELECT phone_number FROM people
        JOIN bank_accounts ON people.id == bank_accounts.person_id
        WHERE bank_accounts.account_number IN (
            SELECT account_number
            FROM atm_transactions
            WHERE year = 2021
            AND month = 7
            AND day = 28
            AND atm_location = "Leggett Street"
            AND transaction_type = "withdraw"
        )
        AND people.license_plate IN (
            -- license plate
            SELECT license_plate
            FROM bakery_security_logs
            WHERE year = 2021
            AND month = 7
            AND day = 28
            AND hour = 10
            AND minute BETWEEN 15 AND 25
            AND activity = "exit"
    )
    )
);

-- Narrowed it down, Theif is either Diana or Bruce
-- Now get the earliest flight from fiftyville on July 29, 2021 and its destination city

-- =======================================================================================
-- New York City: DESTINATION
-- ========================================================================================
SELECT city FROM airports WHERE
id = (
SELECT destination_airport_id FROM flights WHERE
(origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville") AND year = 2021 AND month = 7 AND day = 29) ORDER BY hour, minute LIMIT 1
);

-- Now to chose 1 of Diana and Bruce, get the passengers details of the earliest flight and check whose passport number is there
-- +------+-----------------+
-- | seat | passport_number |
-- +------+-----------------+
-- | 4A   | 5773159633      |
-- +------+-----------------+

-- ================================================================
-- THEIF: Bruce
-- ===================================================================
SELECT seat, passport_number FROM passengers WHERE flight_id =
(
SELECT id FROM flights WHERE
(origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville") AND year = 2021 AND month = 7 AND day = 29)
ORDER BY hour, minute LIMIT 1
) AND
passport_number IN (3592750733, 5773159633);

-- For accomplice, check the reciever where caller is Bruce on July 28 2021 and duration is within a minute
-- +-------+
-- | name  |
-- +-------+
-- | Robin |
-- +-------+

-- ==========================================
-- ACCOMPLICE: Robin
-- ==========================================

SELECT name
FROM people
WHERE phone_number IN (
    SELECT receiver
    FROM  phone_calls
    WHERE year = 2021
    AND month = 7
    AND day = 28
    AND duration < 60
    AND caller = (
        SELECT phone_number
        FROM people
        WHERE name = "Bruce"
    )
);