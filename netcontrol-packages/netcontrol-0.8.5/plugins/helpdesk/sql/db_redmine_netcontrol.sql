-- phpMyAdmin SQL Dump
-- version 3.3.7deb5
-- http://www.phpmyadmin.net
--
-- Servidor: localhost
-- Tempo de Geração: Dez 10, 2012 as 09:30 AM
-- Versão do Servidor: 5.1.49
-- Versão do PHP: 5.3.3-7+squeeze3
CREATE DATABASE IF NOT EXISTS redmine_default;
GRANT ALL PRIVILEGES ON redmine_default.* TO 'netcontrol'@'localhost' IDENTIFIED BY '14c75ad602ce79f4b8fd818c7ba8f88a';

USE redmine_default;
SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Banco de Dados: `redmine_default`
--

-- --------------------------------------------------------

--
-- Estrutura da tabela `attachments`
--

CREATE TABLE IF NOT EXISTS `attachments` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `container_id` int(11) NOT NULL DEFAULT '0',
  `container_type` varchar(30) NOT NULL DEFAULT '',
  `filename` varchar(255) NOT NULL DEFAULT '',
  `disk_filename` varchar(255) NOT NULL DEFAULT '',
  `filesize` int(11) NOT NULL DEFAULT '0',
  `content_type` varchar(255) DEFAULT '',
  `digest` varchar(40) NOT NULL DEFAULT '',
  `downloads` int(11) NOT NULL DEFAULT '0',
  `author_id` int(11) NOT NULL DEFAULT '0',
  `created_on` datetime DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `index_attachments_on_container_id_and_container_type` (`container_id`,`container_type`),
  KEY `index_attachments_on_author_id` (`author_id`),
  KEY `index_attachments_on_created_on` (`created_on`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `attachments`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `auth_sources`
--

CREATE TABLE IF NOT EXISTS `auth_sources` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type` varchar(30) NOT NULL DEFAULT '',
  `name` varchar(60) NOT NULL DEFAULT '',
  `host` varchar(60) DEFAULT NULL,
  `port` int(11) DEFAULT NULL,
  `account` varchar(255) DEFAULT NULL,
  `account_password` varchar(60) DEFAULT NULL,
  `base_dn` varchar(255) DEFAULT NULL,
  `attr_login` varchar(30) DEFAULT NULL,
  `attr_firstname` varchar(30) DEFAULT NULL,
  `attr_lastname` varchar(30) DEFAULT NULL,
  `attr_mail` varchar(30) DEFAULT NULL,
  `onthefly_register` tinyint(1) NOT NULL DEFAULT '0',
  `tls` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `index_auth_sources_on_id_and_type` (`id`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `auth_sources`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `boards`
--

CREATE TABLE IF NOT EXISTS `boards` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `project_id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL DEFAULT '',
  `description` varchar(255) DEFAULT NULL,
  `position` int(11) DEFAULT '1',
  `topics_count` int(11) NOT NULL DEFAULT '0',
  `messages_count` int(11) NOT NULL DEFAULT '0',
  `last_message_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `boards_project_id` (`project_id`),
  KEY `index_boards_on_last_message_id` (`last_message_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `boards`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `changes`
--

CREATE TABLE IF NOT EXISTS `changes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `changeset_id` int(11) NOT NULL,
  `action` varchar(1) NOT NULL DEFAULT '',
  `path` text NOT NULL,
  `from_path` text,
  `from_revision` varchar(255) DEFAULT NULL,
  `revision` varchar(255) DEFAULT NULL,
  `branch` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `changesets_changeset_id` (`changeset_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `changes`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `changesets`
--

CREATE TABLE IF NOT EXISTS `changesets` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `repository_id` int(11) NOT NULL,
  `revision` varchar(255) NOT NULL,
  `committer` varchar(255) DEFAULT NULL,
  `committed_on` datetime NOT NULL,
  `comments` text,
  `commit_date` date DEFAULT NULL,
  `scmid` varchar(255) DEFAULT NULL,
  `user_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `changesets_repos_rev` (`repository_id`,`revision`),
  KEY `index_changesets_on_user_id` (`user_id`),
  KEY `index_changesets_on_repository_id` (`repository_id`),
  KEY `index_changesets_on_committed_on` (`committed_on`),
  KEY `changesets_repos_scmid` (`repository_id`,`scmid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `changesets`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `changesets_issues`
--

CREATE TABLE IF NOT EXISTS `changesets_issues` (
  `changeset_id` int(11) NOT NULL,
  `issue_id` int(11) NOT NULL,
  UNIQUE KEY `changesets_issues_ids` (`changeset_id`,`issue_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Extraindo dados da tabela `changesets_issues`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `comments`
--

CREATE TABLE IF NOT EXISTS `comments` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `commented_type` varchar(30) NOT NULL DEFAULT '',
  `commented_id` int(11) NOT NULL DEFAULT '0',
  `author_id` int(11) NOT NULL DEFAULT '0',
  `comments` text,
  `created_on` datetime NOT NULL,
  `updated_on` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `index_comments_on_commented_id_and_commented_type` (`commented_id`,`commented_type`),
  KEY `index_comments_on_author_id` (`author_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `comments`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `custom_fields`
--

CREATE TABLE IF NOT EXISTS `custom_fields` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type` varchar(30) NOT NULL DEFAULT '',
  `name` varchar(30) NOT NULL DEFAULT '',
  `field_format` varchar(30) NOT NULL DEFAULT '',
  `possible_values` text,
  `regexp` varchar(255) DEFAULT '',
  `min_length` int(11) NOT NULL DEFAULT '0',
  `max_length` int(11) NOT NULL DEFAULT '0',
  `is_required` tinyint(1) NOT NULL DEFAULT '0',
  `is_for_all` tinyint(1) NOT NULL DEFAULT '0',
  `is_filter` tinyint(1) NOT NULL DEFAULT '0',
  `position` int(11) DEFAULT '1',
  `searchable` tinyint(1) DEFAULT '0',
  `default_value` text,
  `editable` tinyint(1) DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `index_custom_fields_on_id_and_type` (`id`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `custom_fields`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `custom_fields_projects`
--

CREATE TABLE IF NOT EXISTS `custom_fields_projects` (
  `custom_field_id` int(11) NOT NULL DEFAULT '0',
  `project_id` int(11) NOT NULL DEFAULT '0',
  KEY `index_custom_fields_projects_on_custom_field_id_and_project_id` (`custom_field_id`,`project_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Extraindo dados da tabela `custom_fields_projects`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `custom_fields_trackers`
--

CREATE TABLE IF NOT EXISTS `custom_fields_trackers` (
  `custom_field_id` int(11) NOT NULL DEFAULT '0',
  `tracker_id` int(11) NOT NULL DEFAULT '0',
  KEY `index_custom_fields_trackers_on_custom_field_id_and_tracker_id` (`custom_field_id`,`tracker_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Extraindo dados da tabela `custom_fields_trackers`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `custom_values`
--

CREATE TABLE IF NOT EXISTS `custom_values` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `customized_type` varchar(30) NOT NULL DEFAULT '',
  `customized_id` int(11) NOT NULL DEFAULT '0',
  `custom_field_id` int(11) NOT NULL DEFAULT '0',
  `value` text,
  PRIMARY KEY (`id`),
  KEY `custom_values_customized` (`customized_type`,`customized_id`),
  KEY `index_custom_values_on_custom_field_id` (`custom_field_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `custom_values`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `documents`
--

CREATE TABLE IF NOT EXISTS `documents` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `project_id` int(11) NOT NULL DEFAULT '0',
  `category_id` int(11) NOT NULL DEFAULT '0',
  `title` varchar(60) NOT NULL DEFAULT '',
  `description` text,
  `created_on` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `documents_project_id` (`project_id`),
  KEY `index_documents_on_category_id` (`category_id`),
  KEY `index_documents_on_created_on` (`created_on`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `documents`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `enabled_modules`
--

CREATE TABLE IF NOT EXISTS `enabled_modules` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `project_id` int(11) DEFAULT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `enabled_modules_project_id` (`project_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=199 ;

--
-- Extraindo dados da tabela `enabled_modules`
--

INSERT INTO `enabled_modules` (`id`, `project_id`, `name`) VALUES
(19, 8, 'issue_tracking'),
(20, 8, 'time_tracking'),
(21, 8, 'news'),
(22, 8, 'documents'),
(23, 8, 'files'),
(24, 8, 'wiki'),
(25, 8, 'repository'),
(26, 8, 'boards'),
(27, 8, 'calendar'),
(28, 8, 'gantt'),
(29, 9, 'issue_tracking'),
(30, 9, 'time_tracking'),
(31, 9, 'news'),
(32, 9, 'documents'),
(33, 9, 'files'),
(34, 9, 'wiki'),
(35, 9, 'repository'),
(36, 9, 'boards'),
(37, 9, 'calendar'),
(38, 9, 'gantt'),
(39, 10, 'issue_tracking'),
(40, 10, 'time_tracking'),
(41, 10, 'news'),
(42, 10, 'documents'),
(43, 10, 'files'),
(44, 10, 'wiki'),
(45, 10, 'repository'),
(46, 10, 'boards'),
(47, 10, 'calendar'),
(48, 10, 'gantt'),
(49, 11, 'issue_tracking'),
(50, 11, 'time_tracking'),
(51, 11, 'news'),
(52, 11, 'documents'),
(53, 11, 'files'),
(54, 11, 'wiki'),
(55, 11, 'repository'),
(56, 11, 'boards'),
(57, 11, 'calendar'),
(58, 11, 'gantt'),
(59, 12, 'issue_tracking'),
(60, 12, 'time_tracking'),
(61, 12, 'news'),
(62, 12, 'documents'),
(63, 12, 'files'),
(64, 12, 'wiki'),
(65, 12, 'repository'),
(66, 12, 'boards'),
(67, 12, 'calendar'),
(68, 12, 'gantt'),
(69, 13, 'issue_tracking'),
(70, 13, 'time_tracking'),
(71, 13, 'news'),
(72, 13, 'documents'),
(73, 13, 'files'),
(74, 13, 'wiki'),
(75, 13, 'repository'),
(76, 13, 'boards'),
(77, 13, 'calendar'),
(78, 13, 'gantt'),
(79, 14, 'issue_tracking'),
(80, 14, 'time_tracking'),
(81, 14, 'news'),
(82, 14, 'documents'),
(83, 14, 'files'),
(84, 14, 'wiki'),
(85, 14, 'repository'),
(86, 14, 'boards'),
(87, 14, 'calendar'),
(88, 14, 'gantt'),
(89, 15, 'issue_tracking'),
(90, 15, 'time_tracking'),
(91, 15, 'news'),
(92, 15, 'documents'),
(93, 15, 'files'),
(94, 15, 'wiki'),
(95, 15, 'repository'),
(96, 15, 'boards'),
(97, 15, 'calendar'),
(98, 15, 'gantt'),
(99, 16, 'issue_tracking'),
(100, 16, 'time_tracking'),
(101, 16, 'news'),
(102, 16, 'documents'),
(103, 16, 'files'),
(104, 16, 'wiki'),
(105, 16, 'repository'),
(106, 16, 'boards'),
(107, 16, 'calendar'),
(108, 16, 'gantt'),
(109, 17, 'issue_tracking'),
(110, 17, 'time_tracking'),
(111, 17, 'news'),
(112, 17, 'documents'),
(113, 17, 'files'),
(114, 17, 'wiki'),
(115, 17, 'repository'),
(116, 17, 'boards'),
(117, 17, 'calendar'),
(118, 17, 'gantt'),
(119, 18, 'issue_tracking'),
(120, 18, 'time_tracking'),
(121, 18, 'news'),
(122, 18, 'documents'),
(123, 18, 'files'),
(124, 18, 'wiki'),
(125, 18, 'repository'),
(126, 18, 'boards'),
(127, 18, 'calendar'),
(128, 18, 'gantt'),
(129, 19, 'issue_tracking'),
(130, 19, 'time_tracking'),
(131, 19, 'news'),
(132, 19, 'documents'),
(133, 19, 'files'),
(134, 19, 'wiki'),
(135, 19, 'repository'),
(136, 19, 'boards'),
(137, 19, 'calendar'),
(138, 19, 'gantt'),
(139, 20, 'issue_tracking'),
(140, 20, 'time_tracking'),
(141, 20, 'news'),
(142, 20, 'documents'),
(143, 20, 'files'),
(144, 20, 'wiki'),
(145, 20, 'repository'),
(146, 20, 'boards'),
(147, 20, 'calendar'),
(148, 20, 'gantt'),
(149, 21, 'issue_tracking'),
(150, 21, 'time_tracking'),
(151, 21, 'news'),
(152, 21, 'documents'),
(153, 21, 'files'),
(154, 21, 'wiki'),
(155, 21, 'repository'),
(156, 21, 'boards'),
(157, 21, 'calendar'),
(158, 21, 'gantt'),
(159, 22, 'issue_tracking'),
(160, 22, 'time_tracking'),
(161, 22, 'news'),
(162, 22, 'documents'),
(163, 22, 'files'),
(164, 22, 'wiki'),
(165, 22, 'repository'),
(166, 22, 'boards'),
(167, 22, 'calendar'),
(168, 22, 'gantt'),
(169, 23, 'issue_tracking'),
(170, 23, 'time_tracking'),
(171, 23, 'news'),
(172, 23, 'documents'),
(173, 23, 'files'),
(174, 23, 'wiki'),
(175, 23, 'repository'),
(176, 23, 'boards'),
(177, 23, 'calendar'),
(178, 23, 'gantt'),
(179, 24, 'issue_tracking'),
(180, 24, 'time_tracking'),
(181, 24, 'news'),
(182, 24, 'documents'),
(183, 24, 'files'),
(184, 24, 'wiki'),
(185, 24, 'repository'),
(186, 24, 'boards'),
(187, 24, 'calendar'),
(188, 24, 'gantt'),
(189, 25, 'issue_tracking'),
(190, 25, 'time_tracking'),
(191, 25, 'news'),
(192, 25, 'documents'),
(193, 25, 'files'),
(194, 25, 'wiki'),
(195, 25, 'repository'),
(196, 25, 'boards'),
(197, 25, 'calendar'),
(198, 25, 'gantt');

-- --------------------------------------------------------

--
-- Estrutura da tabela `enumerations`
--

CREATE TABLE IF NOT EXISTS `enumerations` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(30) NOT NULL DEFAULT '',
  `position` int(11) DEFAULT '1',
  `is_default` tinyint(1) NOT NULL DEFAULT '0',
  `type` varchar(255) DEFAULT NULL,
  `active` tinyint(1) NOT NULL DEFAULT '1',
  `project_id` int(11) DEFAULT NULL,
  `parent_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `index_enumerations_on_project_id` (`project_id`),
  KEY `index_enumerations_on_id_and_type` (`id`,`type`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=10 ;

--
-- Extraindo dados da tabela `enumerations`
--

INSERT INTO `enumerations` (`id`, `name`, `position`, `is_default`, `type`, `active`, `project_id`, `parent_id`) VALUES
(1, 'Documentação de utilizador', 1, 0, 'DocumentCategory', 1, NULL, NULL),
(2, 'Documentação técnica', 2, 0, 'DocumentCategory', 1, NULL, NULL),
(3, 'Baixa', 1, 0, 'IssuePriority', 1, NULL, NULL),
(4, 'Normal', 2, 1, 'IssuePriority', 1, NULL, NULL),
(5, 'Alta', 3, 0, 'IssuePriority', 1, NULL, NULL),
(6, 'Urgente', 4, 0, 'IssuePriority', 1, NULL, NULL),
(7, 'Imediata', 5, 0, 'IssuePriority', 1, NULL, NULL),
(8, 'Planeamento', 1, 0, 'TimeEntryActivity', 1, NULL, NULL),
(9, 'Desenvolvimento', 2, 0, 'TimeEntryActivity', 1, NULL, NULL);

-- --------------------------------------------------------

--
-- Estrutura da tabela `groups_users`
--

CREATE TABLE IF NOT EXISTS `groups_users` (
  `group_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  UNIQUE KEY `groups_users_ids` (`group_id`,`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Extraindo dados da tabela `groups_users`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `issues`
--

CREATE TABLE IF NOT EXISTS `issues` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tracker_id` int(11) NOT NULL DEFAULT '0',
  `project_id` int(11) NOT NULL DEFAULT '0',
  `subject` varchar(255) NOT NULL DEFAULT '',
  `description` text,
  `due_date` date DEFAULT NULL,
  `category_id` int(11) DEFAULT NULL,
  `status_id` int(11) NOT NULL DEFAULT '0',
  `assigned_to_id` int(11) DEFAULT NULL,
  `priority_id` int(11) NOT NULL DEFAULT '0',
  `fixed_version_id` int(11) DEFAULT NULL,
  `author_id` int(11) NOT NULL DEFAULT '0',
  `lock_version` int(11) NOT NULL DEFAULT '0',
  `created_on` datetime DEFAULT NULL,
  `updated_on` datetime DEFAULT NULL,
  `start_date` date DEFAULT NULL,
  `done_ratio` int(11) NOT NULL DEFAULT '0',
  `estimated_hours` float DEFAULT NULL,
  `parent_id` int(11) DEFAULT NULL,
  `root_id` int(11) DEFAULT NULL,
  `lft` int(11) DEFAULT NULL,
  `rgt` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `issues_project_id` (`project_id`),
  KEY `index_issues_on_status_id` (`status_id`),
  KEY `index_issues_on_category_id` (`category_id`),
  KEY `index_issues_on_assigned_to_id` (`assigned_to_id`),
  KEY `index_issues_on_fixed_version_id` (`fixed_version_id`),
  KEY `index_issues_on_tracker_id` (`tracker_id`),
  KEY `index_issues_on_priority_id` (`priority_id`),
  KEY `index_issues_on_author_id` (`author_id`),
  KEY `index_issues_on_created_on` (`created_on`),
  KEY `index_issues_on_root_id_and_lft_and_rgt` (`root_id`,`lft`,`rgt`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=6 ;

--
-- Extraindo dados da tabela `issues`
--

INSERT INTO `issues` (`id`, `tracker_id`, `project_id`, `subject`, `description`, `due_date`, `category_id`, `status_id`, `assigned_to_id`, `priority_id`, `fixed_version_id`, `author_id`, `lock_version`, `created_on`, `updated_on`, `start_date`, `done_ratio`, `estimated_hours`, `parent_id`, `root_id`, `lft`, `rgt`) VALUES
(4, 4, 20, 'hdfh', 'hdf', NULL, NULL, 1, NULL, 4, NULL, 16, 0, '2012-11-28 09:07:53', '2012-11-28 09:07:53', '2012-11-28', 0, NULL, NULL, 4, 1, 2),
(5, 4, 11, 'tefste', 'gdf', NULL, NULL, 1, NULL, 4, NULL, 16, 0, '2012-11-28 14:24:34', '2012-11-28 14:24:34', '2012-11-28', 0, NULL, NULL, 5, 1, 2);

-- --------------------------------------------------------

--
-- Estrutura da tabela `issue_categories`
--

CREATE TABLE IF NOT EXISTS `issue_categories` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `project_id` int(11) NOT NULL DEFAULT '0',
  `name` varchar(30) NOT NULL DEFAULT '',
  `assigned_to_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `issue_categories_project_id` (`project_id`),
  KEY `index_issue_categories_on_assigned_to_id` (`assigned_to_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `issue_categories`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `issue_relations`
--

CREATE TABLE IF NOT EXISTS `issue_relations` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `issue_from_id` int(11) NOT NULL,
  `issue_to_id` int(11) NOT NULL,
  `relation_type` varchar(255) NOT NULL DEFAULT '',
  `delay` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `index_issue_relations_on_issue_from_id` (`issue_from_id`),
  KEY `index_issue_relations_on_issue_to_id` (`issue_to_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `issue_relations`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `issue_statuses`
--

CREATE TABLE IF NOT EXISTS `issue_statuses` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(30) NOT NULL DEFAULT '',
  `is_closed` tinyint(1) NOT NULL DEFAULT '0',
  `is_default` tinyint(1) NOT NULL DEFAULT '0',
  `position` int(11) DEFAULT '1',
  `default_done_ratio` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `index_issue_statuses_on_position` (`position`),
  KEY `index_issue_statuses_on_is_closed` (`is_closed`),
  KEY `index_issue_statuses_on_is_default` (`is_default`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=7 ;

--
-- Extraindo dados da tabela `issue_statuses`
--

INSERT INTO `issue_statuses` (`id`, `name`, `is_closed`, `is_default`, `position`, `default_done_ratio`) VALUES
(1, 'Novo', 0, 1, 1, NULL),
(2, 'In Progress', 0, 0, 2, NULL),
(3, 'Resolvido', 0, 0, 3, NULL),
(4, 'Feedback', 0, 0, 4, NULL),
(5, 'Fechado', 1, 0, 5, NULL),
(6, 'Rejeitado', 1, 0, 6, NULL);

-- --------------------------------------------------------

--
-- Estrutura da tabela `journals`
--

CREATE TABLE IF NOT EXISTS `journals` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `journalized_id` int(11) NOT NULL DEFAULT '0',
  `journalized_type` varchar(30) NOT NULL DEFAULT '',
  `user_id` int(11) NOT NULL DEFAULT '0',
  `notes` text,
  `created_on` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `journals_journalized_id` (`journalized_id`,`journalized_type`),
  KEY `index_journals_on_user_id` (`user_id`),
  KEY `index_journals_on_journalized_id` (`journalized_id`),
  KEY `index_journals_on_created_on` (`created_on`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `journals`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `journal_details`
--

CREATE TABLE IF NOT EXISTS `journal_details` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `journal_id` int(11) NOT NULL DEFAULT '0',
  `property` varchar(30) NOT NULL DEFAULT '',
  `prop_key` varchar(30) NOT NULL DEFAULT '',
  `old_value` varchar(255) DEFAULT NULL,
  `value` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `journal_details_journal_id` (`journal_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `journal_details`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `members`
--

CREATE TABLE IF NOT EXISTS `members` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL DEFAULT '0',
  `project_id` int(11) NOT NULL DEFAULT '0',
  `created_on` datetime DEFAULT NULL,
  `mail_notification` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `index_members_on_user_id` (`user_id`),
  KEY `index_members_on_project_id` (`project_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

--
-- Extraindo dados da tabela `members`
--

INSERT INTO `members` (`id`, `user_id`, `project_id`, `created_on`, `mail_notification`) VALUES
(4, 14, 25, '2012-11-27 15:48:24', 0);

-- --------------------------------------------------------

--
-- Estrutura da tabela `member_roles`
--

CREATE TABLE IF NOT EXISTS `member_roles` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `member_id` int(11) NOT NULL,
  `role_id` int(11) NOT NULL,
  `inherited_from` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `index_member_roles_on_member_id` (`member_id`),
  KEY `index_member_roles_on_role_id` (`role_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

--
-- Extraindo dados da tabela `member_roles`
--

INSERT INTO `member_roles` (`id`, `member_id`, `role_id`, `inherited_from`) VALUES
(4, 4, 6, NULL);

-- --------------------------------------------------------

--
-- Estrutura da tabela `messages`
--

CREATE TABLE IF NOT EXISTS `messages` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `board_id` int(11) NOT NULL,
  `parent_id` int(11) DEFAULT NULL,
  `subject` varchar(255) NOT NULL DEFAULT '',
  `content` text,
  `author_id` int(11) DEFAULT NULL,
  `replies_count` int(11) NOT NULL DEFAULT '0',
  `last_reply_id` int(11) DEFAULT NULL,
  `created_on` datetime NOT NULL,
  `updated_on` datetime NOT NULL,
  `locked` tinyint(1) DEFAULT '0',
  `sticky` int(11) DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `messages_board_id` (`board_id`),
  KEY `messages_parent_id` (`parent_id`),
  KEY `index_messages_on_last_reply_id` (`last_reply_id`),
  KEY `index_messages_on_author_id` (`author_id`),
  KEY `index_messages_on_created_on` (`created_on`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `messages`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `news`
--

CREATE TABLE IF NOT EXISTS `news` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `project_id` int(11) DEFAULT NULL,
  `title` varchar(60) NOT NULL DEFAULT '',
  `summary` varchar(255) DEFAULT '',
  `description` text,
  `author_id` int(11) NOT NULL DEFAULT '0',
  `created_on` datetime DEFAULT NULL,
  `comments_count` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `news_project_id` (`project_id`),
  KEY `index_news_on_author_id` (`author_id`),
  KEY `index_news_on_created_on` (`created_on`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `news`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `open_id_authentication_associations`
--

CREATE TABLE IF NOT EXISTS `open_id_authentication_associations` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `issued` int(11) DEFAULT NULL,
  `lifetime` int(11) DEFAULT NULL,
  `handle` varchar(255) DEFAULT NULL,
  `assoc_type` varchar(255) DEFAULT NULL,
  `server_url` blob,
  `secret` blob,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `open_id_authentication_associations`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `open_id_authentication_nonces`
--

CREATE TABLE IF NOT EXISTS `open_id_authentication_nonces` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `timestamp` int(11) NOT NULL,
  `server_url` varchar(255) DEFAULT NULL,
  `salt` varchar(255) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `open_id_authentication_nonces`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `projects`
--

CREATE TABLE IF NOT EXISTS `projects` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(30) NOT NULL DEFAULT '',
  `description` text,
  `homepage` varchar(255) DEFAULT '',
  `is_public` tinyint(1) NOT NULL DEFAULT '1',
  `parent_id` int(11) DEFAULT NULL,
  `created_on` datetime DEFAULT NULL,
  `updated_on` datetime DEFAULT NULL,
  `identifier` varchar(20) DEFAULT NULL,
  `status` int(11) NOT NULL DEFAULT '1',
  `lft` int(11) DEFAULT NULL,
  `rgt` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `index_projects_on_lft` (`lft`),
  KEY `index_projects_on_rgt` (`rgt`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=26 ;

--
-- Extraindo dados da tabela `projects`
--

INSERT INTO `projects` (`id`, `name`, `description`, `homepage`, `is_public`, `parent_id`, `created_on`, `updated_on`, `identifier`, `status`, `lft`, `rgt`) VALUES
(8, 'Compras', 'Todas os chamados de suporte do departamento de Compras', '', 0, NULL, '2012-11-27 15:22:08', '2012-11-27 15:24:00', 'compras', 1, 13, 14),
(9, 'Recursos Humanos', 'Todas os chamados de suporte do departamento de Recursos Humanos', '', 0, NULL, '2012-11-27 15:23:48', '2012-11-27 15:50:57', 'rh', 1, 29, 30),
(10, 'Secretaría', 'Todos os chamados pertencentes da Secretaría.', '', 0, NULL, '2012-11-27 15:25:19', '2012-11-27 15:33:32', 'secretaria', 1, 31, 32),
(11, 'Call Center ', 'Todos os chamados do Call Center', '', 0, NULL, '2012-11-27 15:34:45', '2012-11-27 15:34:45', 'callcenter', 1, 5, 6),
(12, 'Contabilidade ', 'Todos os chamados da contabilidade', '', 0, NULL, '2012-11-27 15:35:52', '2012-11-27 15:35:52', 'contabilidade', 1, 15, 16),
(13, 'Contabilidade / Auditoria ', 'Todos os chamados do departamento Contabilidade / Auditoria', '', 0, NULL, '2012-11-27 15:36:42', '2012-11-27 15:45:55', 'contauditoria', 1, 17, 18),
(14, 'Contas a pagar ', 'Todos os chamados do departamento Contas a pagar', '', 0, NULL, '2012-11-27 15:37:24', '2012-11-27 15:37:24', 'contasapagar', 1, 19, 20),
(15, 'Diretoria Acadêmica ', 'Todos os chamados da Diretoria Acadêmica ', '', 0, NULL, '2012-11-27 15:37:58', '2012-11-27 15:37:58', 'diretoria', 1, 21, 22),
(16, 'Gestão de Crédito ', 'Todos os chamados Gestão de Crédito', '', 0, NULL, '2012-11-27 15:38:25', '2012-11-27 15:38:25', 'gestao', 1, 25, 26),
(17, 'Presidência', 'Todos chamados da Presidência', '', 0, NULL, '2012-11-27 15:38:51', '2012-11-27 15:38:51', 'presidencia', 1, 27, 28),
(18, 'Transporte', 'Todos chamados para o departamento de Transporte', '', 0, NULL, '2012-11-27 15:39:53', '2012-11-27 15:39:53', 'transporte', 1, 33, 34),
(19, 'Xerox', 'Chamados do Departamento Xerox', '', 0, NULL, '2012-11-27 15:40:38', '2012-11-27 15:40:38', 'xerox', 1, 35, 36),
(20, 'Apoio Acadêmico', 'Chamados Apoio Acadêmico', '', 0, NULL, '2012-11-27 15:41:25', '2012-11-27 17:39:33', 'apoio', 1, 1, 2),
(21, 'Biblioteca', 'Chamados Biblioteca', '', 0, NULL, '2012-11-27 15:41:53', '2012-11-27 15:50:34', 'bibi', 1, 3, 4),
(22, 'Casa do Cidadão', 'Departamento Casa do Cidadão', '', 0, NULL, '2012-11-27 15:42:19', '2012-11-27 15:42:19', 'cdc', 1, 7, 8),
(23, 'Casa dos Professores', 'Chamados Casa dos Professores', '', 0, NULL, '2012-11-27 15:42:52', '2012-11-27 15:42:52', 'professores', 1, 9, 10),
(24, 'Colégio', 'Chamados do colégio', '', 0, NULL, '2012-11-27 15:43:33', '2012-11-27 15:50:44', 'col', 1, 11, 12),
(25, 'Financeiro', 'Chamados Financeiro', '', 0, NULL, '2012-11-27 15:44:18', '2012-11-27 15:44:18', 'finan', 1, 23, 24);

-- --------------------------------------------------------

--
-- Estrutura da tabela `projects_trackers`
--

CREATE TABLE IF NOT EXISTS `projects_trackers` (
  `project_id` int(11) NOT NULL DEFAULT '0',
  `tracker_id` int(11) NOT NULL DEFAULT '0',
  UNIQUE KEY `projects_trackers_unique` (`project_id`,`tracker_id`),
  KEY `projects_trackers_project_id` (`project_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Extraindo dados da tabela `projects_trackers`
--

INSERT INTO `projects_trackers` (`project_id`, `tracker_id`) VALUES
(8, 4),
(8, 5),
(8, 6),
(9, 4),
(9, 5),
(9, 6),
(10, 4),
(10, 5),
(10, 6),
(11, 4),
(11, 5),
(11, 6),
(12, 4),
(12, 5),
(12, 6),
(13, 4),
(13, 5),
(13, 6),
(14, 4),
(14, 5),
(14, 6),
(15, 4),
(15, 5),
(15, 6),
(16, 4),
(16, 5),
(16, 6),
(17, 4),
(17, 5),
(17, 6),
(18, 4),
(18, 5),
(18, 6),
(19, 4),
(19, 5),
(19, 6),
(20, 4),
(20, 5),
(20, 6),
(21, 4),
(21, 5),
(21, 6),
(22, 4),
(22, 5),
(22, 6),
(23, 4),
(23, 5),
(23, 6),
(24, 4),
(24, 5),
(24, 6),
(25, 4),
(25, 5),
(25, 6);

-- --------------------------------------------------------

--
-- Estrutura da tabela `queries`
--

CREATE TABLE IF NOT EXISTS `queries` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `project_id` int(11) DEFAULT NULL,
  `name` varchar(255) NOT NULL DEFAULT '',
  `filters` text,
  `user_id` int(11) NOT NULL DEFAULT '0',
  `is_public` tinyint(1) NOT NULL DEFAULT '0',
  `column_names` text,
  `sort_criteria` text,
  `group_by` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `index_queries_on_project_id` (`project_id`),
  KEY `index_queries_on_user_id` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `queries`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `repositories`
--

CREATE TABLE IF NOT EXISTS `repositories` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `project_id` int(11) NOT NULL DEFAULT '0',
  `url` varchar(255) NOT NULL DEFAULT '',
  `login` varchar(60) DEFAULT '',
  `password` varchar(60) DEFAULT '',
  `root_url` varchar(255) DEFAULT '',
  `type` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `index_repositories_on_project_id` (`project_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `repositories`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `roles`
--

CREATE TABLE IF NOT EXISTS `roles` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(30) NOT NULL DEFAULT '',
  `position` int(11) DEFAULT '1',
  `assignable` tinyint(1) DEFAULT '1',
  `builtin` int(11) NOT NULL DEFAULT '0',
  `permissions` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=8 ;

--
-- Extraindo dados da tabela `roles`
--

INSERT INTO `roles` (`id`, `name`, `position`, `assignable`, `builtin`, `permissions`) VALUES
(1, 'Non member', 1, 1, 1, '--- []\n\n'),
(2, 'Anonymous', 2, 1, 2, '--- []\n\n'),
(6, 'Reporter', 3, 1, 0, '--- \n- :manage_documents\n- :view_documents\n- :manage_files\n- :view_files\n- :manage_categories\n- :view_issues\n- :add_issues\n- :edit_issues\n- :manage_issue_relations\n- :manage_subtasks\n- :add_issue_notes\n- :edit_issue_notes\n- :edit_own_issue_notes\n- :move_issues\n- :delete_issues\n- :manage_public_queries\n- :save_queries\n- :view_issue_watchers\n- :add_issue_watchers\n- :delete_issue_watchers\n'),
(7, 'Administrador', 4, 1, 0, '--- \n- :add_project\n- :edit_project\n- :select_project_modules\n- :manage_members\n- :manage_versions\n- :add_subprojects\n- :view_calendar\n- :manage_documents\n- :view_documents\n- :manage_files\n- :view_files\n- :view_gantt\n- :manage_categories\n- :view_issues\n- :add_issues\n- :edit_issues\n- :manage_issue_relations\n- :manage_subtasks\n- :add_issue_notes\n- :edit_issue_notes\n- :edit_own_issue_notes\n- :move_issues\n- :delete_issues\n- :manage_public_queries\n- :save_queries\n- :view_issue_watchers\n- :add_issue_watchers\n- :delete_issue_watchers\n');

-- --------------------------------------------------------

--
-- Estrutura da tabela `schema_migrations`
--

CREATE TABLE IF NOT EXISTS `schema_migrations` (
  `version` varchar(255) NOT NULL,
  UNIQUE KEY `unique_schema_migrations` (`version`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Extraindo dados da tabela `schema_migrations`
--

INSERT INTO `schema_migrations` (`version`) VALUES
('1'),
('10'),
('100'),
('101'),
('102'),
('103'),
('104'),
('105'),
('106'),
('107'),
('108'),
('11'),
('12'),
('13'),
('14'),
('15'),
('16'),
('17'),
('18'),
('19'),
('2'),
('20'),
('20090214190337'),
('20090312172426'),
('20090312194159'),
('20090318181151'),
('20090323224724'),
('20090401221305'),
('20090401231134'),
('20090403001910'),
('20090406161854'),
('20090425161243'),
('20090503121501'),
('20090503121505'),
('20090503121510'),
('20090614091200'),
('20090704172350'),
('20090704172355'),
('20090704172358'),
('20091010093521'),
('20091017212227'),
('20091017212457'),
('20091017212644'),
('20091017212938'),
('20091017213027'),
('20091017213113'),
('20091017213151'),
('20091017213228'),
('20091017213257'),
('20091017213332'),
('20091017213444'),
('20091017213536'),
('20091017213642'),
('20091017213716'),
('20091017213757'),
('20091017213835'),
('20091017213910'),
('20091017214015'),
('20091017214107'),
('20091017214136'),
('20091017214236'),
('20091017214308'),
('20091017214336'),
('20091017214406'),
('20091017214440'),
('20091017214519'),
('20091017214611'),
('20091017214644'),
('20091017214720'),
('20091017214750'),
('20091025163651'),
('20091108092559'),
('20091114105931'),
('20091123212029'),
('20091205124427'),
('20091220183509'),
('20091220183727'),
('20091220184736'),
('20091225164732'),
('20091227112908'),
('20100221100219'),
('20100313132032'),
('20100313171051'),
('20100705164950'),
('20100819172912'),
('21'),
('22'),
('23'),
('24'),
('25'),
('26'),
('27'),
('28'),
('29'),
('3'),
('30'),
('31'),
('32'),
('33'),
('34'),
('35'),
('36'),
('37'),
('38'),
('39'),
('4'),
('40'),
('41'),
('42'),
('43'),
('44'),
('45'),
('46'),
('47'),
('48'),
('49'),
('5'),
('50'),
('51'),
('52'),
('53'),
('54'),
('55'),
('56'),
('57'),
('58'),
('59'),
('6'),
('60'),
('61'),
('62'),
('63'),
('64'),
('65'),
('66'),
('67'),
('68'),
('69'),
('7'),
('70'),
('71'),
('72'),
('73'),
('74'),
('75'),
('76'),
('77'),
('78'),
('79'),
('8'),
('80'),
('81'),
('82'),
('83'),
('84'),
('85'),
('86'),
('87'),
('88'),
('89'),
('9'),
('90'),
('91'),
('92'),
('93'),
('94'),
('95'),
('96'),
('97'),
('98'),
('99');

-- --------------------------------------------------------

--
-- Estrutura da tabela `settings`
--

CREATE TABLE IF NOT EXISTS `settings` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL DEFAULT '',
  `value` text,
  `updated_on` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `index_settings_on_name` (`name`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=14 ;

--
-- Extraindo dados da tabela `settings`
--

INSERT INTO `settings` (`id`, `name`, `value`, `updated_on`) VALUES
(1, 'feeds_limit', '15', '2012-11-27 16:24:54'),
(2, 'attachment_max_size', '5120', '2012-11-27 16:24:54'),
(3, 'host_name', 'localhost:3000', '2012-11-27 16:24:54'),
(4, 'activity_days_default', '30', '2012-11-27 16:24:54'),
(5, 'protocol', 'http', '2012-11-27 16:24:54'),
(6, 'per_page_options', '25,50,100', '2012-11-27 16:24:54'),
(7, 'file_max_size_displayed', '512', '2012-11-27 16:24:54'),
(8, 'wiki_compression', '', '2012-11-27 16:24:54'),
(9, 'cache_formatted_text', '0', '2012-11-27 16:24:54'),
(10, 'app_title', 'Help Desk', '2012-11-27 16:46:53'),
(11, 'welcome_text', 'h3. Help Desk - Desenvolvido pela Flux Softwares\r\n* site: http://www.fluxsoftwares.com/\r\n* fone: (33) 3322-6309\r\n* email: contato@fluxsoftwares.com', '2012-11-27 17:11:08'),
(12, 'text_formatting', 'textile', '2012-11-27 16:24:54'),
(13, 'diff_max_lines_displayed', '1500', '2012-11-27 16:24:54');

-- --------------------------------------------------------

--
-- Estrutura da tabela `time_entries`
--

CREATE TABLE IF NOT EXISTS `time_entries` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `project_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `issue_id` int(11) DEFAULT NULL,
  `hours` float NOT NULL,
  `comments` varchar(255) DEFAULT NULL,
  `activity_id` int(11) NOT NULL,
  `spent_on` date NOT NULL,
  `tyear` int(11) NOT NULL,
  `tmonth` int(11) NOT NULL,
  `tweek` int(11) NOT NULL,
  `created_on` datetime NOT NULL,
  `updated_on` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `time_entries_project_id` (`project_id`),
  KEY `time_entries_issue_id` (`issue_id`),
  KEY `index_time_entries_on_activity_id` (`activity_id`),
  KEY `index_time_entries_on_user_id` (`user_id`),
  KEY `index_time_entries_on_created_on` (`created_on`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `time_entries`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `tokens`
--

CREATE TABLE IF NOT EXISTS `tokens` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL DEFAULT '0',
  `action` varchar(30) NOT NULL DEFAULT '',
  `value` varchar(40) NOT NULL DEFAULT '',
  `created_on` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `index_tokens_on_user_id` (`user_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=13 ;

--
-- Extraindo dados da tabela `tokens`
--

INSERT INTO `tokens` (`id`, `user_id`, `action`, `value`, `created_on`) VALUES
(2, 1, 'feeds', '3f41c0d0909481c5046b22280fb2831a38b84582', '2012-11-27 14:36:21'),
(4, 3, 'feeds', 'ac3ced1066c1f080088e80fe184c64390d4366c7', '2012-11-27 14:59:52'),
(6, 6, 'feeds', '06be9fc6efec5ad5ba6167fd5af48ce7aa6beca4', '2012-11-27 15:04:08'),
(8, 14, 'feeds', '0fa5ce69c739f7fa05c562ac0308bfaae694aba9', '2012-11-27 15:48:33'),
(10, 15, 'feeds', '32aa8329b8ed6aa308f0d9222ccac634a71438a3', '2012-11-27 15:53:24'),
(12, 16, 'feeds', '1c0cbad7d7454a7085e0b56d81ce8191db16fd29', '2012-11-27 15:54:47');

-- --------------------------------------------------------

--
-- Estrutura da tabela `trackers`
--

CREATE TABLE IF NOT EXISTS `trackers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(30) NOT NULL DEFAULT '',
  `is_in_chlog` tinyint(1) NOT NULL DEFAULT '0',
  `position` int(11) DEFAULT '1',
  `is_in_roadmap` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=8 ;

--
-- Extraindo dados da tabela `trackers`
--

INSERT INTO `trackers` (`id`, `name`, `is_in_chlog`, `position`, `is_in_roadmap`) VALUES
(4, 'Computador', 0, 1, 1),
(5, 'Impressora', 0, 2, 1),
(6, 'Periféricos', 0, 3, 1),
(7, 'outros', 0, 4, 1);

-- --------------------------------------------------------

--
-- Estrutura da tabela `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `login` varchar(30) NOT NULL DEFAULT '',
  `hashed_password` varchar(40) NOT NULL DEFAULT '',
  `firstname` varchar(30) NOT NULL DEFAULT '',
  `lastname` varchar(30) NOT NULL DEFAULT '',
  `mail` varchar(60) NOT NULL DEFAULT '',
  `mail_notification` tinyint(1) NOT NULL DEFAULT '1',
  `admin` tinyint(1) NOT NULL DEFAULT '0',
  `status` int(11) NOT NULL DEFAULT '1',
  `last_login_on` datetime DEFAULT NULL,
  `language` varchar(5) DEFAULT '',
  `auth_source_id` int(11) DEFAULT NULL,
  `created_on` datetime DEFAULT NULL,
  `updated_on` datetime DEFAULT NULL,
  `type` varchar(255) DEFAULT NULL,
  `identity_url` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `index_users_on_id_and_type` (`id`,`type`),
  KEY `index_users_on_auth_source_id` (`auth_source_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=17 ;

--
-- Extraindo dados da tabela `users`
--

INSERT INTO `users` (`id`, `login`, `hashed_password`, `firstname`, `lastname`, `mail`, `mail_notification`, `admin`, `status`, `last_login_on`, `language`, `auth_source_id`, `created_on`, `updated_on`, `type`, `identity_url`) VALUES
(1, 'admin', 'd033e22ae348aeb5660fc2140aec35850c4da997', 'Redmine', 'Admin', 'admin@example.net', 1, 1, 1, '2012-11-27 15:53:38', 'pt-BR', NULL, '2012-11-27 14:34:11', '2012-11-27 15:53:38', 'User', NULL);

-- --------------------------------------------------------

--
-- Estrutura da tabela `user_preferences`
--

CREATE TABLE IF NOT EXISTS `user_preferences` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL DEFAULT '0',
  `others` text,
  `hide_mail` tinyint(1) DEFAULT '0',
  `time_zone` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `index_user_preferences_on_user_id` (`user_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=7 ;

--
-- Extraindo dados da tabela `user_preferences`
--

INSERT INTO `user_preferences` (`id`, `user_id`, `others`, `hide_mail`, `time_zone`) VALUES
(1, 1, '--- \n:no_self_notified: false\n:comments_sorting: asc\n', 0, ''),
(2, 3, '--- {}\n\n', 0, NULL),
(3, 6, '--- {}\n\n', 0, NULL),
(4, 14, '--- {}\n\n', 0, NULL),
(5, 15, '--- {}\n\n', 0, NULL),
(6, 16, '--- \n:gantt_months: 6\n:gantt_zoom: 2\n', 0, NULL);

-- --------------------------------------------------------

--
-- Estrutura da tabela `versions`
--

CREATE TABLE IF NOT EXISTS `versions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `project_id` int(11) NOT NULL DEFAULT '0',
  `name` varchar(255) NOT NULL DEFAULT '',
  `description` varchar(255) DEFAULT '',
  `effective_date` date DEFAULT NULL,
  `created_on` datetime DEFAULT NULL,
  `updated_on` datetime DEFAULT NULL,
  `wiki_page_title` varchar(255) DEFAULT NULL,
  `status` varchar(255) DEFAULT 'open',
  `sharing` varchar(255) NOT NULL DEFAULT 'none',
  PRIMARY KEY (`id`),
  KEY `versions_project_id` (`project_id`),
  KEY `index_versions_on_sharing` (`sharing`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `versions`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `watchers`
--

CREATE TABLE IF NOT EXISTS `watchers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `watchable_type` varchar(255) NOT NULL DEFAULT '',
  `watchable_id` int(11) NOT NULL DEFAULT '0',
  `user_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `watchers_user_id_type` (`user_id`,`watchable_type`),
  KEY `index_watchers_on_user_id` (`user_id`),
  KEY `index_watchers_on_watchable_id_and_watchable_type` (`watchable_id`,`watchable_type`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `watchers`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `wikis`
--

CREATE TABLE IF NOT EXISTS `wikis` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `project_id` int(11) NOT NULL,
  `start_page` varchar(255) NOT NULL,
  `status` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `wikis_project_id` (`project_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=19 ;

--
-- Extraindo dados da tabela `wikis`
--

INSERT INTO `wikis` (`id`, `project_id`, `start_page`, `status`) VALUES
(1, 8, 'Wiki', 1),
(2, 9, 'Wiki', 1),
(3, 10, 'Wiki', 1),
(4, 11, 'Wiki', 1),
(5, 12, 'Wiki', 1),
(6, 13, 'Wiki', 1),
(7, 14, 'Wiki', 1),
(8, 15, 'Wiki', 1),
(9, 16, 'Wiki', 1),
(10, 17, 'Wiki', 1),
(11, 18, 'Wiki', 1),
(12, 19, 'Wiki', 1),
(13, 20, 'Wiki', 1),
(14, 21, 'Wiki', 1),
(15, 22, 'Wiki', 1),
(16, 23, 'Wiki', 1),
(17, 24, 'Wiki', 1),
(18, 25, 'Wiki', 1);

-- --------------------------------------------------------

--
-- Estrutura da tabela `wiki_contents`
--

CREATE TABLE IF NOT EXISTS `wiki_contents` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `page_id` int(11) NOT NULL,
  `author_id` int(11) DEFAULT NULL,
  `text` longtext,
  `comments` varchar(255) DEFAULT '',
  `updated_on` datetime NOT NULL,
  `version` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `wiki_contents_page_id` (`page_id`),
  KEY `index_wiki_contents_on_author_id` (`author_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `wiki_contents`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `wiki_content_versions`
--

CREATE TABLE IF NOT EXISTS `wiki_content_versions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `wiki_content_id` int(11) NOT NULL,
  `page_id` int(11) NOT NULL,
  `author_id` int(11) DEFAULT NULL,
  `data` longblob,
  `compression` varchar(6) DEFAULT '',
  `comments` varchar(255) DEFAULT '',
  `updated_on` datetime NOT NULL,
  `version` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `wiki_content_versions_wcid` (`wiki_content_id`),
  KEY `index_wiki_content_versions_on_updated_on` (`updated_on`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `wiki_content_versions`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `wiki_pages`
--

CREATE TABLE IF NOT EXISTS `wiki_pages` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `wiki_id` int(11) NOT NULL,
  `title` varchar(255) NOT NULL,
  `created_on` datetime NOT NULL,
  `protected` tinyint(1) NOT NULL DEFAULT '0',
  `parent_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `wiki_pages_wiki_id_title` (`wiki_id`,`title`),
  KEY `index_wiki_pages_on_wiki_id` (`wiki_id`),
  KEY `index_wiki_pages_on_parent_id` (`parent_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `wiki_pages`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `wiki_redirects`
--

CREATE TABLE IF NOT EXISTS `wiki_redirects` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `wiki_id` int(11) NOT NULL,
  `title` varchar(255) DEFAULT NULL,
  `redirects_to` varchar(255) DEFAULT NULL,
  `created_on` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `wiki_redirects_wiki_id_title` (`wiki_id`,`title`),
  KEY `index_wiki_redirects_on_wiki_id` (`wiki_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `wiki_redirects`
--


-- --------------------------------------------------------

--
-- Estrutura da tabela `workflows`
--

CREATE TABLE IF NOT EXISTS `workflows` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tracker_id` int(11) NOT NULL DEFAULT '0',
  `old_status_id` int(11) NOT NULL DEFAULT '0',
  `new_status_id` int(11) NOT NULL DEFAULT '0',
  `role_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `wkfs_role_tracker_old_status` (`role_id`,`tracker_id`,`old_status_id`),
  KEY `index_workflows_on_old_status_id` (`old_status_id`),
  KEY `index_workflows_on_role_id` (`role_id`),
  KEY `index_workflows_on_new_status_id` (`new_status_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

--
-- Extraindo dados da tabela `workflows`
--

