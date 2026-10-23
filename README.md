# ghdb-cgi-bin
Green House Database as a cgi-bin application

```
mysql> desc ghdb;
+-----------------------+--------------+------+-----+---------+-------+
| Field                 | Type         | Null | Key | Default | Extra |
+-----------------------+--------------+------+-----+---------+-------+
| plant_name            | varchar(30)  | NO   | PRI | NULL    |       |
| latin_name            | varchar(30)  | YES  |     | NULL    |       |
| height                | decimal(5,2) | YES  |     | NULL    |       |
| width                 | decimal(5,2) | YES  |     | NULL    |       |
| planting_depth        | decimal(5,2) | YES  |     | NULL    |       |
| seed_size             | decimal(5,2) | YES  |     | NULL    |       |
| seed_need_light       | varchar(3)   | YES  |     | NULL    |       |
| seed_scarification    | varchar(3)   | YES  |     | NULL    |       |
| ph                    | decimal(5,2) | YES  |     | NULL    |       |
| ec                    | decimal(5,2) | YES  |     | NULL    |       |
| day_light_interval    | decimal(5,0) | YES  |     | NULL    |       |
| photoperiod_hours     | decimal(5,0) | YES  |     | NULL    |       |
| light_lower           | decimal(5,0) | YES  |     | NULL    |       |
| light_optimal         | decimal(5,0) | YES  |     | NULL    |       |
| light_upper           | decimal(5,0) | YES  |     | NULL    |       |
| nitrogen_lower        | decimal(5,0) | YES  |     | NULL    |       |
| nitrogen_optimal      | decimal(5,0) | YES  |     | NULL    |       |
| nitrogen_upper        | decimal(5,0) | YES  |     | NULL    |       |
| phosphorus_lower      | decimal(5,0) | YES  |     | NULL    |       |
| phosphorus_optimal    | decimal(5,0) | YES  |     | NULL    |       |
| phosphorus_upper      | decimal(5,0) | YES  |     | NULL    |       |
| potassium_lower       | decimal(5,0) | YES  |     | NULL    |       |
| potassium_optimal     | decimal(5,0) | YES  |     | NULL    |       |
| potassium_upper       | decimal(5,0) | YES  |     | NULL    |       |
| germination_lower     | decimal(5,0) | YES  |     | NULL    |       |
| germination_normal    | decimal(5,0) | YES  |     | NULL    |       |
| germination_upper     | decimal(5,0) | YES  |     | NULL    |       |
| transplanting_lower   | decimal(5,0) | YES  |     | NULL    |       |
| transplanting_optimal | decimal(5,0) | YES  |     | NULL    |       |
| transplanting_upper   | decimal(5,0) | YES  |     | NULL    |       |
| maturity_lower        | decimal(5,0) | YES  |     | NULL    |       |
| maturity_optimal      | decimal(5,0) | YES  |     | NULL    |       |
| maturity_upper        | decimal(5,0) | YES  |     | NULL    |       |
| frost_tolerance       | varchar(3)   | YES  |     | NULL    |       |
| flowering             | decimal(5,0) | YES  |     | NULL    |       |
| pollination_primary   | varchar(30)  | YES  |     | NULL    |       |
| pollination_secondary | varchar(30)  | YES  |     | NULL    |       |
+-----------------------+--------------+------+-----+---------+-------+
37 rows in set (0.00 sec)

```
