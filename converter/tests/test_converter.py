import pytest
import os
from src.converter import main
from unittest.mock import patch, mock_open
import argparse


def test_main(mocker):
    mocker.patch(
        "argparse.ArgumentParser.parse_args",
        return_value=argparse.Namespace(
            input_file="input.ninja", output_file="output.manifest"
        ),
    )
    mocker.patch("os.path.isfile", return_value=True)
    mocker.patch("builtins.open", mocker.mock_open())
    mocker.patch("src.parser.Parser.tokenize", return_value=[])
    mocker.patch("src.builder.Builder.get_file_content", return_value="content")

    main()

    open.assert_called_with("output.manifest", "w")
    open().write.assert_called_once_with("content")


def test_main_file_not_found(mocker):
    mocker.patch(
        "argparse.ArgumentParser.parse_args",
        return_value=argparse.Namespace(
            input_file="nonexistent.ninja", output_file="output.manifest"
        ),
    )
    mocker.patch("os.path.isfile", return_value=False)

    with pytest.raises(SystemExit):
        main()


def test_main_no_tokens(mocker):
    mocker.patch(
        "argparse.ArgumentParser.parse_args",
        return_value=argparse.Namespace(
            input_file="input.ninja", output_file="output.manifest"
        ),
    )
    mocker.patch("os.path.isfile", return_value=True)
    mocker.patch("builtins.open", mocker.mock_open())
    mocker.patch("src.parser.Parser.tokenize", return_value=[])
    mocker.patch("src.builder.Builder.get_file_content", return_value="")

    main()

    open.assert_called_with("output.manifest", "w")
    open().write.assert_called_once_with("")
