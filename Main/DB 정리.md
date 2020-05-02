# 개념

1. 데이터 모델 : Structure, Operation, constraint 로 구성된다.
2. relation : unordered set that contains the relationship of attributes that represent entities
3. The set of allowed values for each attribute is called the domain of the attribute.
4. NULL is a member of every domain, indicating that the value is “unknown”
5. However, we implicitly treat them as ordered sets. 데이터베이스는 순서가 있음.
6. Superkey K is a candidate key if K is minimal
7. A foreign key specifies that an attribute from one relation has to map to a tuple in another relation
8. Value in one relation must appear in another relation
9. DDL : How to represent relations and information in a database / Defines database schemas
10. DML : How to store and retrieve information from a database / based on relation algebra
11. Database: a collection of relations (tables)
12. Database schema: the logical structure of the database
13. Database instance: a snapshot of the data in the database at a given instant in time
14. Relation instance: a snapshot of a relation (attributes and tuples) at a given instant in time
15. ![image](https://user-images.githubusercontent.com/42775225/80365292-49b73d80-88c2-11ea-8957-3213eff4ab75.png)

16. Select(행 추출), Project(열 추출)

17. duplicate 사라지게 하는 것 : Projection, Set Operations(Union, INTERSECTION, EXCEPT)

18. Join operation 사용하기

    ![image](https://user-images.githubusercontent.com/42775225/80365893-45d7eb00-88c3-11ea-8509-820bc9c25db0.png)

    ![image](https://user-images.githubusercontent.com/42775225/80365976-6acc5e00-88c3-11ea-9fff-f79de422f86d.png)

19. 합집합 Union하려면, same arity

20. 교집합 Set-Intersection 하려면, same airty, too, 그리고 compatible(양립)하다.

21. 차집합 Set-Difference 하려면, same airty, too, 그리고 compatible(양립)하다.

22. ![image](https://user-images.githubusercontent.com/42775225/80366945-2477fe80-88c5-11ea-8680-9367e1cc26c5.png)

23. ![image](https://user-images.githubusercontent.com/42775225/80367301-d0214e80-88c5-11ea-889f-9dedae27782f.png)
    ![image](https://user-images.githubusercontent.com/42775225/80367346-e29b8800-88c5-11ea-9722-8eee7f6b70ad.png)

24. BETWEEN

    ```mysql
    SELECT name
    FROM instructor
    WHERE salary BETWEEN 90000 AND 100000
    ```

25. PRODUCT

    ```mysql
    SELECT name, course_id
    FROM instructor , teaches 
    WHERE instructor.ID = teaches.ID
    ```

26. NULL

    ```mysql
    SELECT name
    FROM instructor 
    WHERE salary IS NULL	
    ```

27. NOT IN

    ```mysql
    SELECT course_id FROM section
    WHERE semester='Fall'AND year=2017 AND course_id NOT IN
                                                (SELECT course_id FROM section
                                                WHERE semester = 'Spring' AND year = 2018);
                                                
    SELECT DISTINCT name
    FROM instructor
    WHERE name NOT IN ('Mozart', 'Einstein’);
                     
    SELECT COUNT(DISTINCT ID)
    FROM takes
    WHERE (course_id, sec_id, semester, year) IN
                                      (SELECT course_id, sec_id, semester, year FROM teaches
                                      WHERE teaches.ID= 10101);
    ```

    

28. String matching

     • 'Intro%' matches any string beginning with “Intro”
     • '%Comp%' matches any string containing “Comp” as a substring 

     • '_ _ _' matches any string of exactly three characters
     • '_ _ _ %' matches any string of at least three characters

29. ```mysql
    SELECT name
    FROM instructor
    WHERE name LIKE ‘%ri%'
    ```

30. Having : GROUP BY에서 추가적인 predicate

    ```mysql
    SELECT dept_name, AVG(salary) AS avg_salary FROM instructor
    GROUP BY dept_name
    HAVING AVG(salary) > 42000;
    ```

31. SOME

    ```mysql
    Find names of instructors with salary greater than that of SOME (at least one) instructor in the Biology department
    
    SELECT DISTINCT T.name
    FROM instructor AS T, instructor AS S
    WHERE T.salary > S.salary AND S.dept name = 'Biology';
    
    SELECT name
    FROM instructor
    WHERE salary > SOME (SELECT salary
                          FROM instructor
                          WHERE dept name = 'Biology');
    ```

32. ALL

    ```mysql
    Find the names of all instructors whose salary is greater than the salary of all instructors in the Biology department
    
    SELECT name
    FROM instructor
    WHERE salary > ALL (SELECT salary
    FROM instructor
    WHERE dept name = 'Biology');
    ```

33. INSERT

    ```mysql
    INSERT INTO instructor
        SELECT ID, name, dept_name, 18000
        FROM student
        WHERE dept_name = 'Music’ AND total_cred > 144;
    
    INSERT INTO course (course_id, title, dept_name, credits) 
         VALUES ('CS-437', 'Database Systems', 'Comp. Sci.', 4);
    ```

34. DELETE

    ```mysql
    DELETE FROM instructor
    WHERE dept name in (SELECT dept name
    FROM department
    WHERE building = 'Watson');
    ```

35. UPDATE

    ```mysql
    UPDATE instructor
    SET salary = salary * 1.05
    WHERE salary < (SELECT AVG(salary) FROM instructor);
    ```

36. DROP

    ALTER 

    ADD

    ```mysql
    DROP TABLE time_slot_backup;
    ALTER TABLE time_slot_backup ADD remark VARCHAR(20); • 
    ALTER TABLE time_slot_backup DROP remark;
    ```

37. A relationship is an association among several entities

    A relationship set is a mathematical relation among *2이상의 양수 n* entities, each taken from entity sets

38. VIEW 

    (조건)

    : FROM 뒤에 테이블이 1개 일때

    : 집계함수나, DISTINCT를 통해 얻어진 것이 아닌 original attribute를 SELECT할 때

    : SELECT 절에 나열되지 않은 속성은 null로 설정할 수 있음

    : GROUP BY나 HAVING이 없어야 한다.

    ```mysql
    CREATE VIEW physics_fall_2017 AS
    SELECT course.course_id, sec_id, building, room_number FROM course, section
    WHERE course.course_id = section.course_id
    AND course.dept_name = 'Physics' AND section.semester = 'Fall'
    AND section.year = '2017';
    
    CREATE VIEW physics_fall_2017_watson AS SELECT course_id, room_number
    FROM physics_fall_2017
    WHERE building= 'Watson';
    ```

39. UNIQUE

    : Candidate keys are permitted to be null (in contrast to primary keys)

    ```mysql
    CREATE TABLE suppliers (
      supplier_id INT AUTO_INCREMENT,
      name VARCHAR(255),
      phone VARCHAR(15) NOT NULL UNIQUE, address VARCHAR(255),
      PRIMARY KEY (supplier_id),
      UNIQUE (name, address));
      
    
    SELECT T.course_id
    FROM course AS T
    WHERE UNIQUE ( SELECT R.course_id
                    FROM section AS R
                    WHERE T.course_id= R.course_id AND R.year = 2017);
    ```

40. CHECK

    ```mysql
    CREATE TABLE section (course_id VARCHAR(8),
        sec_id VARCHAR(8),
        semester VARCHAR(6) NOT NULL,
        year NUMERIC(4,0),
        building VARCHAR(15),
        room_number VARCHAR(7),
        time_slot_id VARCHAR(4),
        PRIMARY KEY (course_id, sec_id, semester, year), 
        CHECK (semester IN ('Fall', 'Winter', 'Spring', 'Summer')))
    ```

41. CREATE

    ```mysql
    CREATE TABLE course ( 
      (...
          dept_name VARCHAR(20) DEFAULT 'Comp.Sci',
          FOREIGN KEY (dept_name) REFERENCES department
          ON DELETE CASCADE
          ON UPDATE CASCADE, 
       ...)
    ```

42. ACID

    1) Atomic : 하나의 트랜잭션은 여러개의 연산으로 이루어질 수 있는데, 전부가 실행되거나 전부가 실행되지 않거나 해야한다.

    2) Consistent : valid한 데이터만 가져야한다.

    3) Isolated : 여러개의 트랜잭션이 서로가 서로에게 관여하지 않아야한다.

    4) durabel : 커밋이 되면, 사용자가 지우기 전까지 DB에 남아있어야한다.

43. Transactions

    ```mysql
    START TRANSACTION;
    
    SELECT @A:=SUM(salary) FROM instructor 
    		WHERE dept_name='Comp. Sci.'; 
    UPDATE budget_summary SET summary=@A 
    		WHERE dept_name='Comp. Sci.'; 
    		
    COMMIT;
    ```

44. Isolation

    1) Serializable

    2) repeatable read : phantom read

    3) read committed : phantom read, nonrepeatable read

    4) read uncommittted : phantom read, nonrepeatable read, dirty read

![image](https://user-images.githubusercontent.com/42775225/80427755-5f108400-8923-11ea-9dd5-d962e458b0b6.png)

​	1) Phantom read : 트랜잭션 중간에 INSERT가 있는경우(commit)

​	2) nonrepeatable read : 트랜잭션 중간에 UPDATE나 DELETE가 있는경우(commit)

​	3) Dirty read : commit 전에 무슨일이 일어나는 경우

45. CREATE

    ```mysql
    DROP TABLE IF EXISTS `author`;
    CREATE TABLE `author` (
      `id` int(11) NOT NULL AUTO_INCREMENT,
      `name` varchar(45) DEFAULT NULL,
      `profile` varchar(100) DEFAULT NULL,
      PRIMARY KEY (`id`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
    
    INSERT INTO `author` VALUES (1,'kim','developer'),(2,'lee','DBA');
    
    DROP TABLE IF EXISTS `tag`;
    CREATE TABLE `tag` (
      `id` int(11) NOT NULL,
      `name` varchar(45) DEFAULT NULL,
      PRIMARY KEY (`id`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
    
    INSERT INTO `tag` VALUES (1,'rdb'),(2,'free'),(3,'commercial');
    
    DROP TABLE IF EXISTS `topic`;
    CREATE TABLE `topic` (
      `title` varchar(50) NOT NULL,
      `description` text,
      `created` datetime DEFAULT NULL,
      `author_id` int(11) DEFAULT NULL,
      PRIMARY KEY (`title`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
    
    INSERT INTO `topic` VALUES ('MySQL','MySQL is ...','2011-01-01 00:00:00',1),('ORACLE','ORACLE is ...','2012-02-03 00:00:00',1),('SQL Server','SQL Server is ..','2013-01-04 00:00:00',2);
    DROP TABLE IF EXISTS `topic_tag_relation`;
    
    CREATE TABLE `topic_tag_relation` (
      `topic_title` varchar(50) NOT NULL,
      `tag_id` int(11) NOT NULL,
      PRIMARY KEY (`topic_title`,`tag_id`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
    
    INSERT INTO `topic_tag_relation` VALUES ('MySQL',1),('MySQL',2),('ORACLE',1),('ORACLE',3);
    
    DROP TABLE IF EXISTS `topic_type`;
    CREATE TABLE `topic_type` (
      `title` varchar(45) NOT NULL,
      `type` char(6) NOT NULL,
      `price` int(11) DEFAULT NULL,
      PRIMARY KEY (`title`,`type`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8;
    
    INSERT INTO `topic_type` VALUES ('MySQL','online',0),('MySQL','paper',10000),('ORACLE','online',15000);
    ```

    



# Key



### 1. candidate key

 : 여기 안에 속한 key는 Primary key가 될 수 있다.



### 2. primary key

 : tuple을 구별할 수 있는 key



### 3. composite key

 : 두 개 이상의 attribute로 Primary key가 되는 key / the composite key may or maybe not part of FK



### 4. compound key

 : 두 개 이상의 attribute로 Primary key가 되는 key / Any part of the compound key can be FK



### 5. super key

 : 두 개 이상의 attribute로 Primary key가 되는 key / 중복되는 attribute가 있을 수 있다.





# Normalization

### 1NF

 : multivalued data

// 해당 column을 별도의 테이블로 만들고, 기존 테이블과 새로 생성한 테이블을 이어주는 relation테이블을 만든다.

// 이 때, relation 테이블은, 기존 테이블에서 의존하고 있는 ID와 새로 생성한 테이블의 ID를 가진다. (중복가능)



### 2NF

 : primary key 중에서 하나가 다른 걸 determine할 수 있는 경우

 : 특정 column으로 인해서 중복되는 data가 생기는 경우

// 이미 중복되어지고 있는 column과, 중복을 유발하는 column을 모아 다른 테이블로 쪼개야한다. 두 테이블은 모두 기존 테이블의 ID를 가진다.



### 3NF

 : nonPK --> nonPK 경우

// 제 2의 ID역할을 하고 있는 것들의 묶음을 따로 테이블로 때내야한다. 기존 테이블은 제 2의 ID를 추가한다.



### BCNF

 : nonPK --> nonPK 경우 할때, X가 super key중 일부인 경우

 : A non-PK identifies a member of PK: Not in BCNF





### 4NF

 : ID-hobby, ID-favoritefood



# ER-diagram

### primary key는 누가 될 것인가

1. one-one : 둘 중 하나의 key가 primary key가 된다. minimal super key 라는건, 둘 중에 적어도 하나의 key는 가져와야된다는 것이다.
2. one-many : many의 key가 primary key가 된다.
3. many-many : union key가 primary key가 된다.



### weak entity sets

 : own의 관계에 있는 것.




