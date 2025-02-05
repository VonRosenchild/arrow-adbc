# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

import importlib.metadata

import adbc_driver_manager

from ._version import version as __version__

__all__ = ["connect", "__version__"]


def connect(uri: str) -> adbc_driver_manager.AdbcDatabase:
    """Create a low level ADBC connection to Postgres."""
    with importlib.resources.path(
        __package__, "libadbc_driver_postgres.so"
    ) as entrypoint:
        return adbc_driver_manager.AdbcDatabase(driver=str(entrypoint), uri=uri)
