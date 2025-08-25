--
-- PostgreSQL database dump
--

-- Dumped from database version 16.9 (Ubuntu 16.9-0ubuntu0.24.04.1)
-- Dumped by pg_dump version 16.9 (Ubuntu 16.9-0ubuntu0.24.04.1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: adresses; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.adresses (
    adress_id integer NOT NULL,
    adress_name character varying(1000) NOT NULL
);


ALTER TABLE public.adresses OWNER TO postgres;

--
-- Name: adresses_adress_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.adresses_adress_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.adresses_adress_id_seq OWNER TO postgres;

--
-- Name: adresses_adress_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.adresses_adress_id_seq OWNED BY public.adresses.adress_id;


--
-- Name: prices; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.prices (
    price_id integer NOT NULL,
    shop_id integer,
    product_id integer,
    price numeric(12,2) NOT NULL
);


ALTER TABLE public.prices OWNER TO postgres;

--
-- Name: prices_price_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.prices_price_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.prices_price_id_seq OWNER TO postgres;

--
-- Name: prices_price_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.prices_price_id_seq OWNED BY public.prices.price_id;


--
-- Name: products; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.products (
    product_id integer NOT NULL,
    product_name character varying(1000) NOT NULL
);


ALTER TABLE public.products OWNER TO postgres;

--
-- Name: products_product_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.products_product_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.products_product_id_seq OWNER TO postgres;

--
-- Name: products_product_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.products_product_id_seq OWNED BY public.products.product_id;


--
-- Name: quantities; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.quantities (
    quantity_id integer NOT NULL,
    shop_id integer,
    product_id integer,
    quantity integer NOT NULL
);


ALTER TABLE public.quantities OWNER TO postgres;

--
-- Name: quantities_quantity_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.quantities_quantity_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.quantities_quantity_id_seq OWNER TO postgres;

--
-- Name: quantities_quantity_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.quantities_quantity_id_seq OWNED BY public.quantities.quantity_id;


--
-- Name: shops; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.shops (
    shop_id integer NOT NULL,
    shop_name character varying(100) NOT NULL,
    adress_id integer
);


ALTER TABLE public.shops OWNER TO postgres;

--
-- Name: shops_shop_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.shops_shop_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.shops_shop_id_seq OWNER TO postgres;

--
-- Name: shops_shop_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.shops_shop_id_seq OWNED BY public.shops.shop_id;


--
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    user_id integer NOT NULL,
    user_login character varying(50) NOT NULL,
    user_role character varying(20) NOT NULL,
    user_password character varying(255) NOT NULL,
    CONSTRAINT users_user_role_check CHECK (((user_role)::text = ANY ((ARRAY['Пользователь'::character varying, 'Администратор'::character varying])::text[])))
);


ALTER TABLE public.users OWNER TO postgres;

--
-- Name: users_user_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.users_user_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.users_user_id_seq OWNER TO postgres;

--
-- Name: users_user_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.users_user_id_seq OWNED BY public.users.user_id;


--
-- Name: adresses adress_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.adresses ALTER COLUMN adress_id SET DEFAULT nextval('public.adresses_adress_id_seq'::regclass);


--
-- Name: prices price_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.prices ALTER COLUMN price_id SET DEFAULT nextval('public.prices_price_id_seq'::regclass);


--
-- Name: products product_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.products ALTER COLUMN product_id SET DEFAULT nextval('public.products_product_id_seq'::regclass);


--
-- Name: quantities quantity_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.quantities ALTER COLUMN quantity_id SET DEFAULT nextval('public.quantities_quantity_id_seq'::regclass);


--
-- Name: shops shop_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.shops ALTER COLUMN shop_id SET DEFAULT nextval('public.shops_shop_id_seq'::regclass);


--
-- Name: users user_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users ALTER COLUMN user_id SET DEFAULT nextval('public.users_user_id_seq'::regclass);


--
-- Data for Name: adresses; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.adresses (adress_id, adress_name) FROM stdin;
1	ул. Ипподромская, 46
2	ул. Кирова, 110
3	ул. Гоголя, 13
4	ул. Фрунзе, 238
5	ул. Военная, 5
\.


--
-- Data for Name: prices; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.prices (price_id, shop_id, product_id, price) FROM stdin;
1	1	1	71799.00
2	1	2	11999.00
3	1	3	7499.00
4	2	4	8999.00
5	2	5	29899.00
6	3	6	129999.00
7	3	5	27599.00
8	4	7	105999.00
9	5	6	144999.00
\.


--
-- Data for Name: products; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.products (product_id, product_name) FROM stdin;
1	Ноутбук Acer Aspire
2	ПК Inferit Mini
3	Монитор Irbis
4	Xiaomi Redmi A3
5	Apple Watch
6	Ноутбук Thunderobot
7	Системный блок Raskat Strike
\.


--
-- Data for Name: quantities; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.quantities (quantity_id, shop_id, product_id, quantity) FROM stdin;
1	1	1	11
2	1	2	9
3	1	3	27
4	2	4	73
5	2	5	196
6	3	6	42
7	3	5	348
8	4	7	106
9	5	6	18
\.


--
-- Data for Name: shops; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.shops (shop_id, shop_name, adress_id) FROM stdin;
1	DNS	1
2	DNS	2
3	МВидео	3
4	МВидео	4
5	Эльдорадо	5
\.


--
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.users (user_id, user_login, user_role, user_password) FROM stdin;
\.


--
-- Name: adresses_adress_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.adresses_adress_id_seq', 5, true);


--
-- Name: prices_price_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.prices_price_id_seq', 9, true);


--
-- Name: products_product_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.products_product_id_seq', 7, true);


--
-- Name: quantities_quantity_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.quantities_quantity_id_seq', 9, true);


--
-- Name: shops_shop_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.shops_shop_id_seq', 5, true);


--
-- Name: users_user_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.users_user_id_seq', 1, false);


--
-- Name: adresses adresses_adress_name_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.adresses
    ADD CONSTRAINT adresses_adress_name_key UNIQUE (adress_name);


--
-- Name: adresses adresses_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.adresses
    ADD CONSTRAINT adresses_pkey PRIMARY KEY (adress_id);


--
-- Name: prices prices_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.prices
    ADD CONSTRAINT prices_pkey PRIMARY KEY (price_id);


--
-- Name: products products_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.products
    ADD CONSTRAINT products_pkey PRIMARY KEY (product_id);


--
-- Name: products products_product_name_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.products
    ADD CONSTRAINT products_product_name_key UNIQUE (product_name);


--
-- Name: quantities quantities_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.quantities
    ADD CONSTRAINT quantities_pkey PRIMARY KEY (quantity_id);


--
-- Name: shops shops_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.shops
    ADD CONSTRAINT shops_pkey PRIMARY KEY (shop_id);


--
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (user_id);


--
-- Name: users users_user_login_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_user_login_key UNIQUE (user_login);


--
-- Name: prices prices_product_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.prices
    ADD CONSTRAINT prices_product_id_fkey FOREIGN KEY (product_id) REFERENCES public.products(product_id) ON DELETE CASCADE;


--
-- Name: prices prices_shop_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.prices
    ADD CONSTRAINT prices_shop_id_fkey FOREIGN KEY (shop_id) REFERENCES public.shops(shop_id) ON DELETE CASCADE;


--
-- Name: quantities quantities_product_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.quantities
    ADD CONSTRAINT quantities_product_id_fkey FOREIGN KEY (product_id) REFERENCES public.products(product_id) ON DELETE CASCADE;


--
-- Name: quantities quantities_shop_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.quantities
    ADD CONSTRAINT quantities_shop_id_fkey FOREIGN KEY (shop_id) REFERENCES public.shops(shop_id) ON DELETE CASCADE;


--
-- Name: shops shops_adress_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.shops
    ADD CONSTRAINT shops_adress_id_fkey FOREIGN KEY (adress_id) REFERENCES public.adresses(adress_id) ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

