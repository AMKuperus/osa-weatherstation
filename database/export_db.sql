-- phpMyAdmin SQL Dump
-- version 4.9.0.1
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Gegenereerd op: 30 okt 2019 om 19:59
-- Serverversie: 10.2.26-MariaDB-cll-lve
-- PHP-versie: 5.5.38

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `u11798p53315_weather`
--

-- --------------------------------------------------------

--
-- Tabelstructuur voor tabel `Measurements`
--

CREATE TABLE `Measurements` (
  `ID_Measurements` int(11) NOT NULL,
  `ID_Station` int(11) NOT NULL,
  `Datetime` timestamp NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp(),
  `Temperature` float NOT NULL,
  `Pressure` float NOT NULL,
  `Humidity` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Tabelstructuur voor tabel `Station`
--

CREATE TABLE `Station` (
  `ID_Station` int(11) NOT NULL,
  `Naam` text CHARACTER SET ascii NOT NULL,
  `Locatie` text NOT NULL,
  `Opmerking` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexen voor geëxporteerde tabellen
--

--
-- Indexen voor tabel `Measurements`
--
ALTER TABLE `Measurements`
  ADD PRIMARY KEY (`ID_Measurements`),
  ADD KEY `ID_Station` (`ID_Station`);

--
-- Indexen voor tabel `Station`
--
ALTER TABLE `Station`
  ADD PRIMARY KEY (`ID_Station`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
