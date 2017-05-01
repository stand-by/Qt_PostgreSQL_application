--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

--
-- Name: add_contractor_id(); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION add_contractor_id() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
declare
id_var integer;
BEGIN
SELECT nextval('contractor_id_contractor_seq') INTO id_var;
INSERT INTO contractor VALUES (id_var);
NEW.id_contractor := id_var;
RETURN NEW;
END;
$$;


ALTER FUNCTION public.add_contractor_id() OWNER TO eugene;

--
-- Name: add_goods(text, integer); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION add_goods(arg_name text, arg_id_type_of_goods integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
  id_type INT;
BEGIN
  SELECT nextval('goods_id_goods_seq') INTO id_type;
  INSERT INTO goods VALUES (id_type,arg_name,arg_id_type_of_goods);
  RETURN 0;
END;
$$;


ALTER FUNCTION public.add_goods(arg_name text, arg_id_type_of_goods integer) OWNER TO eugene;

--
-- Name: add_juridical_person(text, integer, numeric, text); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION add_juridical_person(arg_name_of_company text, arg_ident_code integer, arg_phone_number numeric, arg_adress text) RETURNS integer
    LANGUAGE plpgsql
    AS $$
BEGIN
  INSERT INTO juridical_person VALUES (arg_name_of_company,arg_ident_code,arg_phone_number,arg_adress,NULL);
  RETURN 0;
END;
$$;


ALTER FUNCTION public.add_juridical_person(arg_name_of_company text, arg_ident_code integer, arg_phone_number numeric, arg_adress text) OWNER TO eugene;

--
-- Name: add_move_order(integer[], integer, integer[], date); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION add_move_order(arg_id_goods_array integer[], arg_document_type_id integer, arg_amount_array integer[], arg_document_date date) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
  id_doc INT;
  id_move_doc INT;
  id_doc_type_move_from_storage_to_shop INT;
  id_doc_type_move_from_shop_to_storage INT;
  amount_at_storage INT;
  amount_at_shop INT;
  name_of_the_good TEXT;
BEGIN
  IF arg_document_date>CURRENT_TIMESTAMP THEN
    RAISE EXCEPTION 'Wrong order date!';
  END IF;

  id_doc_type_move_from_storage_to_shop = (SELECT id_type_of_document FROM type_of_document WHERE name_of_type='Move from storage to shop');
  id_doc_type_move_from_shop_to_storage = (SELECT id_type_of_document FROM type_of_document WHERE name_of_type='Move from shop to storage');

  IF arg_document_type_id = id_doc_type_move_from_storage_to_shop THEN
    SELECT nextval('document_id_document_seq') INTO id_doc;
    INSERT INTO document VALUES (id_doc,arg_document_date,1,arg_document_type_id,2);

    FOR i IN array_lower(arg_id_goods_array, 1) .. array_upper(arg_id_goods_array, 1)
    LOOP
      SELECT count_good_amount_at_storage(arg_id_goods_array[i]) INTO amount_at_storage;
      SELECT count_good_amount_at_shop(arg_id_goods_array[i]) INTO amount_at_shop;

      SELECT name_of_goods FROM goods WHERE id_goods=arg_id_goods_array[i] INTO name_of_the_good;

      SELECT nextval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_') INTO id_move_doc;

      IF amount_at_storage < arg_amount_array[i] THEN
        PERFORM setval('document_id_document_seq',currval('document_id_document_seq')-1);
        PERFORM setval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_',currval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_')-i);
        RAISE EXCEPTION 'There is not enough % product in the storage to transport to the shop',name_of_the_good;
      END IF;

      INSERT INTO document_on_the_move_of_goods VALUES (arg_amount_array[i],0,id_doc,arg_id_goods_array[i],id_move_doc);
    END LOOP;
    RETURN 0;
  END IF;

  IF arg_document_type_id = id_doc_type_move_from_shop_to_storage THEN
    SELECT nextval('document_id_document_seq') INTO id_doc;
    INSERT INTO document VALUES (id_doc,arg_document_date,2,arg_document_type_id,1);

    FOR i IN array_lower(arg_id_goods_array, 1) .. array_upper(arg_id_goods_array, 1)
    LOOP
      SELECT count_good_amount_at_storage(arg_id_goods_array[i]) INTO amount_at_storage;
      SELECT count_good_amount_at_shop(arg_id_goods_array[i]) INTO amount_at_shop;

      SELECT name_of_goods FROM goods WHERE id_goods=arg_id_goods_array[i] INTO name_of_the_good;

      SELECT nextval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_') INTO id_move_doc;

      IF amount_at_shop < arg_amount_array[i] THEN
        PERFORM setval('document_id_document_seq',currval('document_id_document_seq')-1);
        PERFORM setval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_',currval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_')-i);
        RAISE EXCEPTION 'There is not enough % product in the shop to transport to the storage',name_of_the_good;
      END IF;

      INSERT INTO document_on_the_move_of_goods VALUES (arg_amount_array[i],0,id_doc,arg_id_goods_array[i],id_move_doc);
    END LOOP;
    RETURN 0;
  END IF;

  RAISE EXCEPTION 'Transportation has not been happened';
END;
$$;


ALTER FUNCTION public.add_move_order(arg_id_goods_array integer[], arg_document_type_id integer, arg_amount_array integer[], arg_document_date date) OWNER TO eugene;

--
-- Name: add_physical_person(text, text, text, date, text, integer, integer); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION add_physical_person(arg_surname text, arg_name text, arg_pathronymic text, arg_birth_date date, arg_adress text, arg_ident_code integer, arg_phone_number integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
BEGIN
  INSERT INTO physical_person VALUES (arg_surname,arg_name,arg_pathronymic,arg_birth_date,arg_adress,arg_ident_code,arg_phone_number,NULL);
  RETURN 0;
END;
$$;


ALTER FUNCTION public.add_physical_person(arg_surname text, arg_name text, arg_pathronymic text, arg_birth_date date, arg_adress text, arg_ident_code integer, arg_phone_number integer) OWNER TO eugene;

--
-- Name: add_purchase_order(integer, integer[], integer[], numeric[], date); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION add_purchase_order(arg_id_contractor integer, arg_id_goods_array integer[], arg_amount_array integer[], arg_price_array numeric[], arg_document_date date) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
  id_doc INT;
  id_move_doc INT;
  id_doc_type INT;
BEGIN
  IF arg_document_date>CURRENT_TIMESTAMP THEN
    RAISE EXCEPTION 'Wrong order date!';
  END IF;

  id_doc_type = (SELECT id_type_of_document FROM type_of_document WHERE name_of_type='Purchase to the storage');
  SELECT nextval('document_id_document_seq') INTO id_doc;
  INSERT INTO document VALUES (id_doc,arg_document_date,arg_id_contractor,id_doc_type,1);

  FOR i IN array_lower(arg_id_goods_array, 1) .. array_upper(arg_id_goods_array, 1)
  LOOP
    SELECT nextval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_') INTO id_move_doc;
    INSERT INTO document_on_the_move_of_goods VALUES (arg_amount_array[i],arg_price_array[i],id_doc,arg_id_goods_array[i],id_move_doc);
  END LOOP;

  RETURN 0;
END;
$$;


ALTER FUNCTION public.add_purchase_order(arg_id_contractor integer, arg_id_goods_array integer[], arg_amount_array integer[], arg_price_array numeric[], arg_document_date date) OWNER TO eugene;

--
-- Name: add_sell_order(integer, integer[], integer[], numeric[], date); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION add_sell_order(arg_id_contractor integer, arg_id_goods_array integer[], arg_amount_array integer[], arg_price_array numeric[], arg_document_date date) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
  id_doc INT;
  id_move_doc INT;
  id_doc_type INT;
  amount_at_storage INT;
  amount_at_shop INT;
  name_of_the_good  TEXT;
BEGIN
  IF arg_document_date>CURRENT_TIMESTAMP THEN
    RAISE EXCEPTION 'Wrong order date!';
  END IF;

  id_doc_type = (SELECT id_type_of_document FROM type_of_document WHERE name_of_type='Sale from the store');
  SELECT nextval('document_id_document_seq') INTO id_doc;
  INSERT INTO document VALUES (id_doc,arg_document_date,2,id_doc_type,arg_id_contractor);

  FOR i IN array_lower(arg_id_goods_array, 1) .. array_upper(arg_id_goods_array, 1)
  LOOP
    SELECT count_good_amount_at_storage(arg_id_goods_array[i]) INTO amount_at_storage;
    SELECT count_good_amount_at_shop(arg_id_goods_array[i]) INTO amount_at_shop;
    SELECT name_of_goods FROM goods WHERE id_goods=arg_id_goods_array[i] INTO name_of_the_good;

    SELECT nextval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_') INTO id_move_doc;

    IF amount_at_shop+amount_at_storage < arg_amount_array[i] THEN
      PERFORM setval('document_id_document_seq',currval('document_id_document_seq')-1);
      PERFORM setval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_',currval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_')-i);
      RAISE EXCEPTION 'It is not enough % product, should contact the supplier',name_of_the_good;
    ELSEIF amount_at_shop < arg_amount_array[i] AND amount_at_shop+amount_at_storage >= arg_amount_array[i] THEN
      PERFORM setval('document_id_document_seq',currval('document_id_document_seq')-1);
      PERFORM setval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_',currval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_')-i);
      RAISE EXCEPTION 'It is not enough % product in the shop, but it is present in the storage. Need to transport from storage to shop',name_of_the_good;
    ELSE
      INSERT INTO document_on_the_move_of_goods VALUES (arg_amount_array[i],arg_price_array[i],id_doc,arg_id_goods_array[i],id_move_doc);
    END IF;
  END LOOP;
  RETURN 0;
END;
$$;


ALTER FUNCTION public.add_sell_order(arg_id_contractor integer, arg_id_goods_array integer[], arg_amount_array integer[], arg_price_array numeric[], arg_document_date date) OWNER TO eugene;

--
-- Name: add_type_of_goods(text); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION add_type_of_goods(arg_name text) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
  id_type INT;
BEGIN
  SELECT nextval('type_of_goods_id_type_of_goods_seq') INTO id_type;
  INSERT INTO type_of_goods VALUES (id_type,arg_name);
  RETURN currval('type_of_goods_id_type_of_goods_seq');
END;
$$;


ALTER FUNCTION public.add_type_of_goods(arg_name text) OWNER TO eugene;

--
-- Name: calculate_minimal_price_for_product(integer); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION calculate_minimal_price_for_product(arg_id_good integer) RETURNS numeric
    LANGUAGE plpgsql
    AS $$
DECLARE
  n INT;
  date DATE;
  type INT;
  amount INT;
  price NUMERIC;
  sum NUMERIC;

  curr_min NUMERIC;
  total_price NUMERIC;
  total_amount INT;
BEGIN
  SELECT count(*) FROM summary_on_product(arg_id_good) INTO n;
  RAISE NOTICE '%',n;

  IF n=0 THEN
    RETURN 1.00;
  END IF;

  curr_min = 0;
  total_amount = 0;
  total_price = 0;
  FOR date, type, amount, price, sum IN
     (SELECT * FROM summary_on_product(arg_id_good))
  LOOP
    RAISE NOTICE '%, %, %, %, %',date, type, amount, price, sum;

    IF type=1 THEN
      total_price = total_price + sum;
      total_amount = total_amount + amount;
      RAISE NOTICE 'total_amount = % total_price = %',total_amount,total_price;
    END IF;
    IF type=2 THEN
      curr_min = total_price/total_amount;
      RAISE NOTICE 'curr_min = %',curr_min;
      total_amount = total_amount - amount;
      total_price = total_price - (curr_min*amount);
      RAISE NOTICE 'total_amount = % total_price = %',total_amount,total_price;
    END IF;

  END LOOP;

  curr_min = total_price/total_amount;
  RETURN curr_min;
END;
$$;


ALTER FUNCTION public.calculate_minimal_price_for_product(arg_id_good integer) OWNER TO eugene;

--
-- Name: count_good_amount_at_shop(integer); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION count_good_amount_at_shop(arg_id_goods integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
  purchased_to_storage INT;
  sold_from_shop INT;
  moved_from_storage_to_shop INT;
  moved_from_shop_to_storage INT;
BEGIN
  moved_from_storage_to_shop = (SELECT amount FROM goods_amount_for_each_document_type WHERE id_goods=arg_id_goods AND id_type_of_document=3);
  IF moved_from_storage_to_shop IS NULL THEN moved_from_storage_to_shop=0;
  END IF;
  moved_from_shop_to_storage = (SELECT amount FROM goods_amount_for_each_document_type WHERE id_goods=arg_id_goods AND id_type_of_document=4);
  IF moved_from_shop_to_storage IS NULL THEN moved_from_shop_to_storage=0;
  END IF;
  sold_from_shop = (SELECT amount FROM goods_amount_for_each_document_type WHERE id_goods=arg_id_goods AND id_type_of_document=2);
  IF sold_from_shop IS NULL THEN sold_from_shop=0;
  END IF;
  RETURN (moved_from_storage_to_shop-moved_from_shop_to_storage-sold_from_shop);
END;
$$;


ALTER FUNCTION public.count_good_amount_at_shop(arg_id_goods integer) OWNER TO eugene;

--
-- Name: count_good_amount_at_storage(integer); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION count_good_amount_at_storage(arg_id_goods integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
  purchased_to_storage INT;
  sold_from_shop INT;
  moved_from_storage_to_shop INT;
  moved_from_shop_to_storage INT;
BEGIN
  purchased_to_storage = (SELECT amount FROM goods_amount_for_each_document_type WHERE id_goods=arg_id_goods AND id_type_of_document=1);
  IF purchased_to_storage IS NULL THEN purchased_to_storage=0;
  END IF;
  moved_from_storage_to_shop = (SELECT amount FROM goods_amount_for_each_document_type WHERE id_goods=arg_id_goods AND id_type_of_document=3);
  IF moved_from_storage_to_shop IS NULL THEN moved_from_storage_to_shop=0;
  END IF;
  moved_from_shop_to_storage = (SELECT amount FROM goods_amount_for_each_document_type WHERE id_goods=arg_id_goods AND id_type_of_document=4);
  IF moved_from_shop_to_storage IS NULL THEN moved_from_shop_to_storage=0;
  END IF;
  RETURN (purchased_to_storage+moved_from_shop_to_storage-moved_from_storage_to_shop);
END;
$$;


ALTER FUNCTION public.count_good_amount_at_storage(arg_id_goods integer) OWNER TO eugene;

--
-- Name: goods_details_on_document(integer); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION goods_details_on_document(arg_id_doc integer) RETURNS TABLE(res_id_document_on_the_move_of_goods integer, res_name_of_goods character varying, res_amount integer, res_price numeric)
    LANGUAGE plpgsql
    AS $$
BEGIN --SELECT round (2/3::DECIMAL, 2)::TEXT
  RETURN QUERY SELECT id_document_on_the_move_of_goods, name_of_goods, amount, (SELECT  round(price::DECIMAL,2))
  FROM document_on_the_move_of_goods
  LEFT JOIN goods ON document_on_the_move_of_goods.id_goods = goods.id_goods
  WHERE document_on_the_move_of_goods.id_document = arg_id_doc
  ORDER BY id_document_on_the_move_of_goods;
END;
$$;


ALTER FUNCTION public.goods_details_on_document(arg_id_doc integer) OWNER TO eugene;

--
-- Name: summary_on_product(integer); Type: FUNCTION; Schema: public; Owner: eugene
--

CREATE FUNCTION summary_on_product(arg_id_goods integer) RETURNS TABLE(document_date date, id_type_of_document integer, amount integer, price numeric, total numeric)
    LANGUAGE plpgsql STABLE
    AS $$
BEGIN

FOR document_date, id_type_of_document, amount, price IN
   SELECT document.document_date, document.id_type_of_document, d.amount, d.price
   FROM   document_on_the_move_of_goods d
   LEFT JOIN document ON document.id_document=d.id_document
   WHERE  d.id_goods = arg_id_goods AND (document.id_type_of_document = 1 OR document.id_type_of_document = 2)
   ORDER  BY document.document_date
LOOP
   total := amount * price;
   RETURN NEXT;
END LOOP;

END
$$;


ALTER FUNCTION public.summary_on_product(arg_id_goods integer) OWNER TO eugene;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: contractor; Type: TABLE; Schema: public; Owner: eugene; Tablespace: 
--

CREATE TABLE contractor (
    id_contractor integer NOT NULL
);


ALTER TABLE public.contractor OWNER TO eugene;

--
-- Name: contractor_id_contractor_seq; Type: SEQUENCE; Schema: public; Owner: eugene
--

CREATE SEQUENCE contractor_id_contractor_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.contractor_id_contractor_seq OWNER TO eugene;

--
-- Name: contractor_id_contractor_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: eugene
--

ALTER SEQUENCE contractor_id_contractor_seq OWNED BY contractor.id_contractor;


--
-- Name: juridical_person; Type: TABLE; Schema: public; Owner: eugene; Tablespace: 
--

CREATE TABLE juridical_person (
    name_of_company_or_organization character varying(30),
    identification_code integer,
    phone_number numeric(15,0),
    adress character varying(30),
    id_contractor integer NOT NULL
);


ALTER TABLE public.juridical_person OWNER TO eugene;

--
-- Name: physical_person; Type: TABLE; Schema: public; Owner: eugene; Tablespace: 
--

CREATE TABLE physical_person (
    surname character varying(30),
    name character varying(30),
    pathronymic character varying(30),
    date_of_birth date,
    adress character varying(30),
    identificational_code integer,
    phone_number integer,
    id_contractor integer NOT NULL
);


ALTER TABLE public.physical_person OWNER TO eugene;

--
-- Name: contractors; Type: VIEW; Schema: public; Owner: eugene
--

CREATE VIEW contractors AS
 SELECT physical_person.id_contractor,
    physical_person.surname AS name,
    physical_person.adress,
    physical_person.identificational_code AS code,
    physical_person.phone_number AS phone,
    'physical'::text AS table_type
   FROM physical_person
UNION
 SELECT juridical_person.id_contractor,
    juridical_person.name_of_company_or_organization AS name,
    juridical_person.adress,
    juridical_person.identification_code AS code,
    juridical_person.phone_number AS phone,
    'juridical'::text AS table_type
   FROM juridical_person
  ORDER BY 1;


ALTER TABLE public.contractors OWNER TO eugene;

--
-- Name: department; Type: TABLE; Schema: public; Owner: eugene; Tablespace: 
--

CREATE TABLE department (
    name_of_company_or_organization character varying(30),
    identification_code integer,
    phone_number numeric(15,0),
    adress character varying(30),
    id_contractor integer NOT NULL
);


ALTER TABLE public.department OWNER TO eugene;

--
-- Name: document; Type: TABLE; Schema: public; Owner: eugene; Tablespace: 
--

CREATE TABLE document (
    id_document integer NOT NULL,
    document_date date,
    id_contractor_from integer,
    id_type_of_document integer,
    id_contractor_to integer
);


ALTER TABLE public.document OWNER TO eugene;

--
-- Name: document_id_document_seq; Type: SEQUENCE; Schema: public; Owner: eugene
--

CREATE SEQUENCE document_id_document_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.document_id_document_seq OWNER TO eugene;

--
-- Name: document_id_document_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: eugene
--

ALTER SEQUENCE document_id_document_seq OWNED BY document.id_document;


--
-- Name: document_on_the_move_of_goods; Type: TABLE; Schema: public; Owner: eugene; Tablespace: 
--

CREATE TABLE document_on_the_move_of_goods (
    amount integer,
    price numeric(10,2),
    id_document integer NOT NULL,
    id_goods integer,
    id_document_on_the_move_of_goods integer NOT NULL
);


ALTER TABLE public.document_on_the_move_of_goods OWNER TO eugene;

--
-- Name: document_on_the_move_of_goods_id_document_on_the_move_of_goods_; Type: SEQUENCE; Schema: public; Owner: eugene
--

CREATE SEQUENCE document_on_the_move_of_goods_id_document_on_the_move_of_goods_
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.document_on_the_move_of_goods_id_document_on_the_move_of_goods_ OWNER TO eugene;

--
-- Name: document_on_the_move_of_goods_id_document_on_the_move_of_goods_; Type: SEQUENCE OWNED BY; Schema: public; Owner: eugene
--

ALTER SEQUENCE document_on_the_move_of_goods_id_document_on_the_move_of_goods_ OWNED BY document_on_the_move_of_goods.id_document_on_the_move_of_goods;


--
-- Name: goods; Type: TABLE; Schema: public; Owner: eugene; Tablespace: 
--

CREATE TABLE goods (
    id_goods integer NOT NULL,
    name_of_goods character varying(30),
    id_type_of_goods integer
);


ALTER TABLE public.goods OWNER TO eugene;

--
-- Name: goods_amount_for_each_document_type; Type: VIEW; Schema: public; Owner: eugene
--

CREATE VIEW goods_amount_for_each_document_type AS
 SELECT document_on_the_move_of_goods.id_goods,
    document.id_type_of_document,
    sum(document_on_the_move_of_goods.amount) AS amount
   FROM (document_on_the_move_of_goods
     LEFT JOIN document ON ((document_on_the_move_of_goods.id_document = document.id_document)))
  GROUP BY document_on_the_move_of_goods.id_goods, document.id_type_of_document
  ORDER BY document_on_the_move_of_goods.id_goods, document.id_type_of_document;


ALTER TABLE public.goods_amount_for_each_document_type OWNER TO eugene;

--
-- Name: goods_id_goods_seq; Type: SEQUENCE; Schema: public; Owner: eugene
--

CREATE SEQUENCE goods_id_goods_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.goods_id_goods_seq OWNER TO eugene;

--
-- Name: goods_id_goods_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: eugene
--

ALTER SEQUENCE goods_id_goods_seq OWNED BY goods.id_goods;


--
-- Name: type_of_goods; Type: TABLE; Schema: public; Owner: eugene; Tablespace: 
--

CREATE TABLE type_of_goods (
    id_type_of_goods integer NOT NULL,
    name_of_type character varying(30)
);


ALTER TABLE public.type_of_goods OWNER TO eugene;

--
-- Name: goods_list; Type: VIEW; Schema: public; Owner: eugene
--

CREATE VIEW goods_list AS
 SELECT goods.id_goods,
    goods.name_of_goods,
    type_of_goods.name_of_type
   FROM (goods
     LEFT JOIN type_of_goods ON ((goods.id_type_of_goods = type_of_goods.id_type_of_goods)))
  ORDER BY goods.id_goods;


ALTER TABLE public.goods_list OWNER TO eugene;

--
-- Name: type_of_document; Type: TABLE; Schema: public; Owner: eugene; Tablespace: 
--

CREATE TABLE type_of_document (
    id_type_of_document integer NOT NULL,
    name_of_type character varying(30)
);


ALTER TABLE public.type_of_document OWNER TO eugene;

--
-- Name: short_move_goods; Type: VIEW; Schema: public; Owner: eugene
--

CREATE VIEW short_move_goods AS
 SELECT document.id_document,
    document.document_date,
    type_of_document.name_of_type
   FROM (document
     LEFT JOIN type_of_document ON ((document.id_type_of_document = type_of_document.id_type_of_document)))
  WHERE (((type_of_document.name_of_type)::text = 'Move from storage to shop'::text) OR ((type_of_document.name_of_type)::text = 'Move from shop to storage'::text))
  ORDER BY document.id_document;


ALTER TABLE public.short_move_goods OWNER TO eugene;

--
-- Name: short_purchase_goods; Type: VIEW; Schema: public; Owner: eugene
--

CREATE VIEW short_purchase_goods AS
 SELECT document.id_document,
    document.document_date,
    juridical_person.name_of_company_or_organization AS supplier,
    department.name_of_company_or_organization AS receiver
   FROM (((document
     LEFT JOIN type_of_document ON ((document.id_type_of_document = type_of_document.id_type_of_document)))
     LEFT JOIN juridical_person ON ((document.id_contractor_from = juridical_person.id_contractor)))
     LEFT JOIN department ON ((document.id_contractor_to = department.id_contractor)))
  WHERE ((type_of_document.name_of_type)::text = 'Purchase to the storage'::text)
  ORDER BY document.id_document;


ALTER TABLE public.short_purchase_goods OWNER TO eugene;

--
-- Name: short_sell_goods; Type: VIEW; Schema: public; Owner: eugene
--

CREATE VIEW short_sell_goods AS
 SELECT document.id_document,
    document.document_date,
    department.name_of_company_or_organization AS supplier,
    contractors.name AS receiver
   FROM (((document
     LEFT JOIN type_of_document ON ((document.id_type_of_document = type_of_document.id_type_of_document)))
     LEFT JOIN ( SELECT physical_person.id_contractor,
            physical_person.surname AS name
           FROM physical_person
        UNION
         SELECT juridical_person.id_contractor,
            juridical_person.name_of_company_or_organization AS name
           FROM juridical_person) contractors ON ((document.id_contractor_to = contractors.id_contractor)))
     LEFT JOIN department ON ((document.id_contractor_from = department.id_contractor)))
  WHERE ((type_of_document.name_of_type)::text = 'Sale from the store'::text)
  ORDER BY document.id_document;


ALTER TABLE public.short_sell_goods OWNER TO eugene;

--
-- Name: type_of_goods_id_type_of_goods_seq; Type: SEQUENCE; Schema: public; Owner: eugene
--

CREATE SEQUENCE type_of_goods_id_type_of_goods_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.type_of_goods_id_type_of_goods_seq OWNER TO eugene;

--
-- Name: type_of_goods_id_type_of_goods_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: eugene
--

ALTER SEQUENCE type_of_goods_id_type_of_goods_seq OWNED BY type_of_goods.id_type_of_goods;


--
-- Name: id_contractor; Type: DEFAULT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY contractor ALTER COLUMN id_contractor SET DEFAULT nextval('contractor_id_contractor_seq'::regclass);


--
-- Name: id_document; Type: DEFAULT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY document ALTER COLUMN id_document SET DEFAULT nextval('document_id_document_seq'::regclass);


--
-- Name: id_document_on_the_move_of_goods; Type: DEFAULT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY document_on_the_move_of_goods ALTER COLUMN id_document_on_the_move_of_goods SET DEFAULT nextval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_'::regclass);


--
-- Name: id_goods; Type: DEFAULT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY goods ALTER COLUMN id_goods SET DEFAULT nextval('goods_id_goods_seq'::regclass);


--
-- Name: id_type_of_goods; Type: DEFAULT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY type_of_goods ALTER COLUMN id_type_of_goods SET DEFAULT nextval('type_of_goods_id_type_of_goods_seq'::regclass);


--
-- Data for Name: contractor; Type: TABLE DATA; Schema: public; Owner: eugene
--

COPY contractor (id_contractor) FROM stdin;
1
2
3
4
5
\.


--
-- Name: contractor_id_contractor_seq; Type: SEQUENCE SET; Schema: public; Owner: eugene
--

SELECT pg_catalog.setval('contractor_id_contractor_seq', 5, true);


--
-- Data for Name: department; Type: TABLE DATA; Schema: public; Owner: eugene
--

COPY department (name_of_company_or_organization, identification_code, phone_number, adress, id_contractor) FROM stdin;
Storage	1	442363987	Kiev city	1
Shop	2	442048193	Kiev city	2
\.


--
-- Data for Name: document; Type: TABLE DATA; Schema: public; Owner: eugene
--

COPY document (id_document, document_date, id_contractor_from, id_type_of_document, id_contractor_to) FROM stdin;
6	2017-05-01	1	3	2
7	2017-05-01	2	2	5
1	2017-05-01	3	1	1
2	2017-05-01	3	1	1
3	2017-05-01	3	1	1
4	2017-05-01	3	1	1
5	2017-05-01	1	3	2
\.


--
-- Name: document_id_document_seq; Type: SEQUENCE SET; Schema: public; Owner: eugene
--

SELECT pg_catalog.setval('document_id_document_seq', 7, true);


--
-- Data for Name: document_on_the_move_of_goods; Type: TABLE DATA; Schema: public; Owner: eugene
--

COPY document_on_the_move_of_goods (amount, price, id_document, id_goods, id_document_on_the_move_of_goods) FROM stdin;
10	1000.00	1	1	1
10	100.00	1	5	2
1	1000.00	2	1	3
1	900.00	3	1	4
10	500.00	4	2	5
10	100.00	4	4	6
10	0.00	5	1	7
10	0.00	5	4	8
2	0.00	6	1	9
5	999.00	7	1	10
1	200.00	7	4	11
\.


--
-- Name: document_on_the_move_of_goods_id_document_on_the_move_of_goods_; Type: SEQUENCE SET; Schema: public; Owner: eugene
--

SELECT pg_catalog.setval('document_on_the_move_of_goods_id_document_on_the_move_of_goods_', 11, true);


--
-- Data for Name: goods; Type: TABLE DATA; Schema: public; Owner: eugene
--

COPY goods (id_goods, name_of_goods, id_type_of_goods) FROM stdin;
1	Engine for Nissan	1
2	Engine for Ford	1
3	Honda transmission	2
4	Pioneer	3
5	Nissan transmission	1
\.


--
-- Name: goods_id_goods_seq; Type: SEQUENCE SET; Schema: public; Owner: eugene
--

SELECT pg_catalog.setval('goods_id_goods_seq', 5, true);


--
-- Data for Name: juridical_person; Type: TABLE DATA; Schema: public; Owner: eugene
--

COPY juridical_person (name_of_company_or_organization, identification_code, phone_number, adress, id_contractor) FROM stdin;
Official Nissan Distributor	123456	448756978	Kiev	3
Ford Motors	468786523	445879632	Seattle	4
\.


--
-- Data for Name: physical_person; Type: TABLE DATA; Schema: public; Owner: eugene
--

COPY physical_person (surname, name, pathronymic, date_of_birth, adress, identificational_code, phone_number, id_contractor) FROM stdin;
Johnson	Bob	Peter	1900-01-01	London	654865	4878666	5
\.


--
-- Data for Name: type_of_document; Type: TABLE DATA; Schema: public; Owner: eugene
--

COPY type_of_document (id_type_of_document, name_of_type) FROM stdin;
1	Purchase to the storage
2	Sale from the store
3	Move from storage to shop
4	Move from shop to storage
\.


--
-- Data for Name: type_of_goods; Type: TABLE DATA; Schema: public; Owner: eugene
--

COPY type_of_goods (id_type_of_goods, name_of_type) FROM stdin;
1	Engine
2	Transmission
3	Audio system
\.


--
-- Name: type_of_goods_id_type_of_goods_seq; Type: SEQUENCE SET; Schema: public; Owner: eugene
--

SELECT pg_catalog.setval('type_of_goods_id_type_of_goods_seq', 3, true);


--
-- Name: Key1; Type: CONSTRAINT; Schema: public; Owner: eugene; Tablespace: 
--

ALTER TABLE ONLY contractor
    ADD CONSTRAINT "Key1" PRIMARY KEY (id_contractor);


--
-- Name: Key13; Type: CONSTRAINT; Schema: public; Owner: eugene; Tablespace: 
--

ALTER TABLE ONLY document
    ADD CONSTRAINT "Key13" PRIMARY KEY (id_document);


--
-- Name: Key14; Type: CONSTRAINT; Schema: public; Owner: eugene; Tablespace: 
--

ALTER TABLE ONLY type_of_document
    ADD CONSTRAINT "Key14" PRIMARY KEY (id_type_of_document);


--
-- Name: Key18; Type: CONSTRAINT; Schema: public; Owner: eugene; Tablespace: 
--

ALTER TABLE ONLY goods
    ADD CONSTRAINT "Key18" PRIMARY KEY (id_goods);


--
-- Name: Key19; Type: CONSTRAINT; Schema: public; Owner: eugene; Tablespace: 
--

ALTER TABLE ONLY type_of_goods
    ADD CONSTRAINT "Key19" PRIMARY KEY (id_type_of_goods);


--
-- Name: Key2; Type: CONSTRAINT; Schema: public; Owner: eugene; Tablespace: 
--

ALTER TABLE ONLY physical_person
    ADD CONSTRAINT "Key2" PRIMARY KEY (id_contractor);


--
-- Name: Key3; Type: CONSTRAINT; Schema: public; Owner: eugene; Tablespace: 
--

ALTER TABLE ONLY juridical_person
    ADD CONSTRAINT "Key3" PRIMARY KEY (id_contractor);


--
-- Name: department_id_contractor_pk; Type: CONSTRAINT; Schema: public; Owner: eugene; Tablespace: 
--

ALTER TABLE ONLY department
    ADD CONSTRAINT department_id_contractor_pk PRIMARY KEY (id_contractor);


--
-- Name: document_on_the_move_of_goods_pkey; Type: CONSTRAINT; Schema: public; Owner: eugene; Tablespace: 
--

ALTER TABLE ONLY document_on_the_move_of_goods
    ADD CONSTRAINT document_on_the_move_of_goods_pkey PRIMARY KEY (id_document_on_the_move_of_goods);


--
-- Name: IX_Relationship1; Type: INDEX; Schema: public; Owner: eugene; Tablespace: 
--

CREATE INDEX "IX_Relationship1" ON document_on_the_move_of_goods USING btree (id_goods);


--
-- Name: IX_Relationship14; Type: INDEX; Schema: public; Owner: eugene; Tablespace: 
--

CREATE INDEX "IX_Relationship14" ON document USING btree (id_contractor_from);


--
-- Name: IX_Relationship15; Type: INDEX; Schema: public; Owner: eugene; Tablespace: 
--

CREATE INDEX "IX_Relationship15" ON document USING btree (id_type_of_document);


--
-- Name: IX_Relationship21; Type: INDEX; Schema: public; Owner: eugene; Tablespace: 
--

CREATE INDEX "IX_Relationship21" ON goods USING btree (id_type_of_goods);


--
-- Name: ix_relationship16; Type: INDEX; Schema: public; Owner: eugene; Tablespace: 
--

CREATE INDEX ix_relationship16 ON document USING btree (id_contractor_to);


--
-- Name: when_add_juridical_person; Type: TRIGGER; Schema: public; Owner: eugene
--

CREATE TRIGGER when_add_juridical_person BEFORE INSERT ON juridical_person FOR EACH ROW EXECUTE PROCEDURE add_contractor_id();


--
-- Name: when_add_physical_person; Type: TRIGGER; Schema: public; Owner: eugene
--

CREATE TRIGGER when_add_physical_person BEFORE INSERT ON physical_person FOR EACH ROW EXECUTE PROCEDURE add_contractor_id();


--
-- Name: department_contractor_id_contractor_fk; Type: FK CONSTRAINT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY department
    ADD CONSTRAINT department_contractor_id_contractor_fk FOREIGN KEY (id_contractor) REFERENCES contractor(id_contractor);


--
-- Name: document_id_contractor_from_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY document
    ADD CONSTRAINT document_id_contractor_from_fkey FOREIGN KEY (id_contractor_from) REFERENCES contractor(id_contractor);


--
-- Name: document_id_contractor_to_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY document
    ADD CONSTRAINT document_id_contractor_to_fkey FOREIGN KEY (id_contractor_to) REFERENCES contractor(id_contractor);


--
-- Name: document_id_type_of_document_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY document
    ADD CONSTRAINT document_id_type_of_document_fkey FOREIGN KEY (id_type_of_document) REFERENCES type_of_document(id_type_of_document);


--
-- Name: document_on_the_move_of_goods_id_document_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY document_on_the_move_of_goods
    ADD CONSTRAINT document_on_the_move_of_goods_id_document_fkey FOREIGN KEY (id_document) REFERENCES document(id_document);


--
-- Name: document_on_the_move_of_goods_id_goods_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY document_on_the_move_of_goods
    ADD CONSTRAINT document_on_the_move_of_goods_id_goods_fkey FOREIGN KEY (id_goods) REFERENCES goods(id_goods);


--
-- Name: goods_id_type_of_goods_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY goods
    ADD CONSTRAINT goods_id_type_of_goods_fkey FOREIGN KEY (id_type_of_goods) REFERENCES type_of_goods(id_type_of_goods);


--
-- Name: juridical_person_id_contractor_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY juridical_person
    ADD CONSTRAINT juridical_person_id_contractor_fkey FOREIGN KEY (id_contractor) REFERENCES contractor(id_contractor);


--
-- Name: physical_person_id_contractor_fkey; Type: FK CONSTRAINT; Schema: public; Owner: eugene
--

ALTER TABLE ONLY physical_person
    ADD CONSTRAINT physical_person_id_contractor_fkey FOREIGN KEY (id_contractor) REFERENCES contractor(id_contractor);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- Name: goods_details_on_document(integer); Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON FUNCTION goods_details_on_document(arg_id_doc integer) FROM PUBLIC;
REVOKE ALL ON FUNCTION goods_details_on_document(arg_id_doc integer) FROM eugene;
GRANT ALL ON FUNCTION goods_details_on_document(arg_id_doc integer) TO eugene;
GRANT ALL ON FUNCTION goods_details_on_document(arg_id_doc integer) TO manager;
GRANT ALL ON FUNCTION goods_details_on_document(arg_id_doc integer) TO PUBLIC;


--
-- Name: contractor; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE contractor FROM PUBLIC;
REVOKE ALL ON TABLE contractor FROM eugene;
GRANT ALL ON TABLE contractor TO eugene;
GRANT ALL ON TABLE contractor TO vendor;
GRANT SELECT ON TABLE contractor TO inspector;
GRANT ALL ON TABLE contractor TO manager;


--
-- Name: contractor_id_contractor_seq; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON SEQUENCE contractor_id_contractor_seq FROM PUBLIC;
REVOKE ALL ON SEQUENCE contractor_id_contractor_seq FROM eugene;
GRANT ALL ON SEQUENCE contractor_id_contractor_seq TO eugene;
GRANT ALL ON SEQUENCE contractor_id_contractor_seq TO manager;
GRANT SELECT,USAGE ON SEQUENCE contractor_id_contractor_seq TO vendor;


--
-- Name: juridical_person; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE juridical_person FROM PUBLIC;
REVOKE ALL ON TABLE juridical_person FROM eugene;
GRANT ALL ON TABLE juridical_person TO eugene;
GRANT ALL ON TABLE juridical_person TO vendor;
GRANT SELECT ON TABLE juridical_person TO inspector;
GRANT ALL ON TABLE juridical_person TO manager;


--
-- Name: physical_person; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE physical_person FROM PUBLIC;
REVOKE ALL ON TABLE physical_person FROM eugene;
GRANT ALL ON TABLE physical_person TO eugene;
GRANT ALL ON TABLE physical_person TO vendor;
GRANT SELECT ON TABLE physical_person TO inspector;
GRANT ALL ON TABLE physical_person TO manager;


--
-- Name: contractors; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE contractors FROM PUBLIC;
REVOKE ALL ON TABLE contractors FROM eugene;
GRANT ALL ON TABLE contractors TO eugene;
GRANT ALL ON TABLE contractors TO manager;
GRANT SELECT ON TABLE contractors TO vendor;


--
-- Name: department; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE department FROM PUBLIC;
REVOKE ALL ON TABLE department FROM eugene;
GRANT ALL ON TABLE department TO eugene;
GRANT ALL ON TABLE department TO manager;


--
-- Name: document; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE document FROM PUBLIC;
REVOKE ALL ON TABLE document FROM eugene;
GRANT ALL ON TABLE document TO eugene;
GRANT ALL ON TABLE document TO vendor;
GRANT SELECT ON TABLE document TO inspector;
GRANT ALL ON TABLE document TO manager;


--
-- Name: document_id_document_seq; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON SEQUENCE document_id_document_seq FROM PUBLIC;
REVOKE ALL ON SEQUENCE document_id_document_seq FROM eugene;
GRANT ALL ON SEQUENCE document_id_document_seq TO eugene;
GRANT ALL ON SEQUENCE document_id_document_seq TO manager;
GRANT ALL ON SEQUENCE document_id_document_seq TO vendor;


--
-- Name: document_on_the_move_of_goods; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE document_on_the_move_of_goods FROM PUBLIC;
REVOKE ALL ON TABLE document_on_the_move_of_goods FROM eugene;
GRANT ALL ON TABLE document_on_the_move_of_goods TO eugene;
GRANT ALL ON TABLE document_on_the_move_of_goods TO vendor;
GRANT SELECT ON TABLE document_on_the_move_of_goods TO inspector;
GRANT ALL ON TABLE document_on_the_move_of_goods TO manager;


--
-- Name: document_on_the_move_of_goods_id_document_on_the_move_of_goods_; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON SEQUENCE document_on_the_move_of_goods_id_document_on_the_move_of_goods_ FROM PUBLIC;
REVOKE ALL ON SEQUENCE document_on_the_move_of_goods_id_document_on_the_move_of_goods_ FROM eugene;
GRANT ALL ON SEQUENCE document_on_the_move_of_goods_id_document_on_the_move_of_goods_ TO eugene;
GRANT ALL ON SEQUENCE document_on_the_move_of_goods_id_document_on_the_move_of_goods_ TO manager;
GRANT ALL ON SEQUENCE document_on_the_move_of_goods_id_document_on_the_move_of_goods_ TO vendor;


--
-- Name: goods; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE goods FROM PUBLIC;
REVOKE ALL ON TABLE goods FROM eugene;
GRANT ALL ON TABLE goods TO eugene;
GRANT ALL ON TABLE goods TO vendor;
GRANT SELECT ON TABLE goods TO inspector;
GRANT ALL ON TABLE goods TO manager;


--
-- Name: goods_amount_for_each_document_type; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE goods_amount_for_each_document_type FROM PUBLIC;
REVOKE ALL ON TABLE goods_amount_for_each_document_type FROM eugene;
GRANT ALL ON TABLE goods_amount_for_each_document_type TO eugene;
GRANT ALL ON TABLE goods_amount_for_each_document_type TO manager;
GRANT SELECT ON TABLE goods_amount_for_each_document_type TO vendor;
GRANT SELECT ON TABLE goods_amount_for_each_document_type TO inspector;


--
-- Name: goods_id_goods_seq; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON SEQUENCE goods_id_goods_seq FROM PUBLIC;
REVOKE ALL ON SEQUENCE goods_id_goods_seq FROM eugene;
GRANT ALL ON SEQUENCE goods_id_goods_seq TO eugene;
GRANT ALL ON SEQUENCE goods_id_goods_seq TO manager;
GRANT SELECT,USAGE ON SEQUENCE goods_id_goods_seq TO vendor;


--
-- Name: type_of_goods; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE type_of_goods FROM PUBLIC;
REVOKE ALL ON TABLE type_of_goods FROM eugene;
GRANT ALL ON TABLE type_of_goods TO eugene;
GRANT ALL ON TABLE type_of_goods TO vendor;
GRANT SELECT ON TABLE type_of_goods TO inspector;
GRANT ALL ON TABLE type_of_goods TO manager;


--
-- Name: goods_list; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE goods_list FROM PUBLIC;
REVOKE ALL ON TABLE goods_list FROM eugene;
GRANT ALL ON TABLE goods_list TO eugene;
GRANT ALL ON TABLE goods_list TO manager;
GRANT SELECT ON TABLE goods_list TO vendor;
GRANT SELECT ON TABLE goods_list TO inspector;


--
-- Name: type_of_document; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE type_of_document FROM PUBLIC;
REVOKE ALL ON TABLE type_of_document FROM eugene;
GRANT ALL ON TABLE type_of_document TO eugene;
GRANT SELECT ON TABLE type_of_document TO inspector;
GRANT SELECT ON TABLE type_of_document TO vendor;
GRANT ALL ON TABLE type_of_document TO manager;


--
-- Name: short_move_goods; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE short_move_goods FROM PUBLIC;
REVOKE ALL ON TABLE short_move_goods FROM eugene;
GRANT ALL ON TABLE short_move_goods TO eugene;
GRANT SELECT ON TABLE short_move_goods TO manager;


--
-- Name: short_purchase_goods; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE short_purchase_goods FROM PUBLIC;
REVOKE ALL ON TABLE short_purchase_goods FROM eugene;
GRANT ALL ON TABLE short_purchase_goods TO eugene;
GRANT SELECT ON TABLE short_purchase_goods TO manager;


--
-- Name: short_sell_goods; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON TABLE short_sell_goods FROM PUBLIC;
REVOKE ALL ON TABLE short_sell_goods FROM eugene;
GRANT ALL ON TABLE short_sell_goods TO eugene;
GRANT SELECT ON TABLE short_sell_goods TO manager;


--
-- Name: type_of_goods_id_type_of_goods_seq; Type: ACL; Schema: public; Owner: eugene
--

REVOKE ALL ON SEQUENCE type_of_goods_id_type_of_goods_seq FROM PUBLIC;
REVOKE ALL ON SEQUENCE type_of_goods_id_type_of_goods_seq FROM eugene;
GRANT ALL ON SEQUENCE type_of_goods_id_type_of_goods_seq TO eugene;
GRANT ALL ON SEQUENCE type_of_goods_id_type_of_goods_seq TO manager;
GRANT SELECT,USAGE ON SEQUENCE type_of_goods_id_type_of_goods_seq TO vendor;


--
-- Name: DEFAULT PRIVILEGES FOR TABLES; Type: DEFAULT ACL; Schema: public; Owner: postgres
--

ALTER DEFAULT PRIVILEGES FOR ROLE postgres IN SCHEMA public REVOKE ALL ON TABLES  FROM PUBLIC;
ALTER DEFAULT PRIVILEGES FOR ROLE postgres IN SCHEMA public REVOKE ALL ON TABLES  FROM postgres;
ALTER DEFAULT PRIVILEGES FOR ROLE postgres IN SCHEMA public GRANT SELECT ON TABLES  TO eugene;
ALTER DEFAULT PRIVILEGES FOR ROLE postgres IN SCHEMA public GRANT SELECT ON TABLES  TO manager;


--
-- Name: DEFAULT PRIVILEGES FOR TABLES; Type: DEFAULT ACL; Schema: public; Owner: eugene
--

ALTER DEFAULT PRIVILEGES FOR ROLE eugene IN SCHEMA public REVOKE ALL ON TABLES  FROM PUBLIC;
ALTER DEFAULT PRIVILEGES FOR ROLE eugene IN SCHEMA public REVOKE ALL ON TABLES  FROM eugene;
ALTER DEFAULT PRIVILEGES FOR ROLE eugene IN SCHEMA public GRANT SELECT ON TABLES  TO eugene;
ALTER DEFAULT PRIVILEGES FOR ROLE eugene IN SCHEMA public GRANT SELECT ON TABLES  TO manager;


--
-- PostgreSQL database dump complete
--

