# Используем базовый образ Ubuntu 24.04
FROM ubuntu:24.04

# Установка необходимых зависимостей
RUN apt-get update && apt-get install -y \
    g++ \
    libpq-dev \
    libpqxx-dev \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# Создание пользователя tester
ARG UID=100001
ARG GID=100001
RUN groupadd -g ${GID} tester && \
    useradd -m -u ${UID} -g tester tester

# Установка рабочей директории
WORKDIR /app

# Копируем исходные файлы
COPY server/ ./server/
COPY config/config.json ./config/config.json
COPY dump.sql ./dump.sql

# Установка прав на директорию приложения
RUN chown -R tester:tester /app

# Переключаемся на пользователя tester
USER tester

# Компиляция сервера
WORKDIR /app/server
RUN g++ server.cpp -o server -lpqxx -lpq -lcrypto -lssl

# Возвращаемся в корневую директорию
WORKDIR /app

# Запуск сервера
CMD ["./server/server"]
